#include <iostream>
#include <unistd.h>
#include <timer.h>
#include <thread>
#include <redis.h>
#include "mpi.h"
#include <fcntl.h>

std::string client_gen_random(const int len) {
    std::string tmp_s;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    srand( (unsigned) time(NULL) * getpid());
    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

int main(int argc, char*argv[]){
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string buffer = client_gen_random(16*1024*1024);
    uint64_t limit = (uint64_t)2*1024*1024*1024;

    std::string remote_host = "";

    std::shared_ptr<redis_client> redis_memory, redis_nvme, pfs_redis;

    //    config.topic_+"_LDMS"
    redis_memory = std::make_shared<redis_client>(remote_host, "MEMORY");
    redis_nvme = std::make_shared<redis_client>(remote_host, "NVME");
    pfs_redis = std::make_shared<redis_client>(remote_host, "PFS_CAP");

    int file_memory = open("/tmp/jaime_apollo/test", O_RDWR | O_CREAT, 0644);
    int file_nvme = open("/mnt/nvme/jcernudagarcia/apollo_nvme/test", O_RDWR | O_CREAT, 0644);
    int file_ssd = open("/mnt/nvme/jcernudagarcia/pvfs2-mount/test", O_RDWR | O_CREAT, 0644);

    Timer pop_timer;
    pop_timer.startTime();

    while(std::stod(redis_memory->subscribe_last().back().second.back().second) < limit){
        write(file_memory, buffer.c_str(), buffer.length());
    }
    while(std::stod(redis_nvme->subscribe_last().back().second.back().second) < limit){
        write(file_nvme, buffer.c_str(), buffer.length());
    }
    while(std::stod(pfs_redis->subscribe_last().back().second.back().second) < limit){
        write(file_ssd, buffer.c_str(), buffer.length());
    }

    pop_timer.endTimeWithPrint("Real Test " + std::to_string(world_rank) + " :");

    MPI_Finalize();
    return 0;
}

