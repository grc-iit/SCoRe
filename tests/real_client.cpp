#include <iostream>
#include <unistd.h>
#include <timer.h>
#include <thread>
#include <redis.h>
#include <fcntl.h>
#include <mpi.h>

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
    int comm_size, id;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    std::string buffer = client_gen_random(16*1024*1024);
    uint64_t limit = (uint64_t)2*1024*1024*1024;

    std::string remote_host;
    if(argc == 2) {
        remote_host = argv[1];
    }
    else exit(1);

    std::cout << remote_host << " " << comm_size << " " << id << std::endl;
    std::shared_ptr<redis_client> redis_memory, redis_nvme, pfs_redis;

    //    config.topic_+"_LDMS"
    redis_memory = std::make_shared<redis_client>("tcp://localhost", "MEMORY_"+ std::to_string(id));
    redis_nvme = std::make_shared<redis_client>("tcp://localhost", "NVME_"+ std::to_string(id));
    pfs_redis = std::make_shared<redis_client>(remote_host, "PFS_CAP");

    int file_memory = open("/mnt/nvme/jcernudagarcia/tempfs/test_mem", O_RDWR | O_CREAT, 0644);
    int file_nvme = open("/mnt/nvme/jcernudagarcia/apollo_nvme/test_nvme", O_RDWR | O_CREAT, 0644);
    int file_ssd = open("/mnt/nvme/jcernudagarcia/pvfs2-mount/test_ssd", O_RDWR | O_CREAT, 0644);

    Timer pop_timer;
    pop_timer.startTime();

    auto start_memory = std::stod(redis_memory->subscribe_last().back().second.back().second);
    std::cout << "Memory (" + std::to_string(start_memory) + ")" << std::endl;
    while(std::stod(redis_memory->subscribe_last().back().second.back().second) - start_memory < limit){
        write(file_memory, buffer.c_str(), buffer.length());
    }
    std::cout << "Done Memory" << std::endl;

    auto start_nvme = std::stod(redis_nvme->subscribe_last().back().second.back().second);
    std::cout << "NVMe (" + std::to_string(start_nvme) + ")" << std::endl;
    while(std::stod(redis_nvme->subscribe_last().back().second.back().second) - start_nvme < limit){
        write(file_nvme, buffer.c_str(), buffer.length());
    }
    std::cout << "Done NVME" << std::endl;

    auto start_ssd = std::stod(pfs_redis->subscribe_last().back().second.back().second);
    std::cout << "SSD (" + std::to_string(start_ssd) + " , " + std::to_string(comm_size * limit) + ")"<< std::endl;
    while(std::stod(pfs_redis->subscribe_last().back().second.back().second) - start_ssd < comm_size * limit){
        write(file_ssd, buffer.c_str(), buffer.length());
    }
    std::cout << "Done SSD" << std::endl;

    pop_timer.endTimeWithPrint("Real Test ");

    MPI_Finalize();
    return 0;
}

