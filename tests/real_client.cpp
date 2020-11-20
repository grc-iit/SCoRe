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

void do_io(const std::shared_ptr<redis_client>& redis_memory, const std::shared_ptr<redis_client>& redis_nvme,
           const std::shared_ptr<redis_client>& pfs_redis, std::string buffer,
           int process_id, int thread_id, int comm_size){
    uint64_t limit = (uint64_t)1*1024*1024*1024;

    Timer nvme_timer, ssd_timer, end_timer;

    std::string memory_path = "/mnt/nvme/jcernudagarcia/tempfs/test_mem_" + std::to_string(thread_id);
    std::string nvme_path = "/mnt/nvme/jcernudagarcia/apollo_nvme/test_nvme_" + std::to_string(thread_id);
    std::string ssd_path = "/mnt/nvme/jcernudagarcia/pvfs2-mount/test_ssd_" + std::to_string(process_id) + "_"+ std::to_string(thread_id);
    int file_memory = open(memory_path.c_str(), O_RDWR | O_CREAT, 0644);
    int file_nvme = open(nvme_path.c_str(), O_RDWR | O_CREAT, 0644);
    int file_ssd = open(ssd_path.c_str(), O_RDWR | O_CREAT, 0644);

    auto start_memory = std::stod(redis_memory->subscribe_last().back().second.back().second);
//    std::cout << "Memory (" + std::to_string(start_memory) + ")" << std::endl;
    while(std::stod(redis_memory->subscribe_last().back().second.back().second) - start_memory < 20*limit){
        write(file_memory, buffer.c_str(), buffer.length());
    }
//    std::cout << "Done Memory" << std::endl;
    nvme_timer.startTime();
//    MPI_Barrier(MPI_COMM_WORLD);

    auto start_nvme = std::stod(redis_nvme->subscribe_last().back().second.back().second);
//    std::cout << "NVMe (" + std::to_string(start_nvme) + ")" << std::endl;
    while(std::stod(redis_nvme->subscribe_last().back().second.back().second) - start_nvme < 40 * limit){
        write(file_nvme, buffer.c_str(), buffer.length());
    }

    nvme_timer.endTimeWithPrint("Id " + std::to_string(process_id) + "_" + std::to_string(thread_id) + ": ");

    ssd_timer.startTime();
//    MPI_Barrier(MPI_COMM_WORLD);

    auto start_ssd = std::stod(pfs_redis->subscribe_last().back().second.back().second);
//    std::cout << "SSD (" + std::to_string(start_ssd) + " , " + std::to_string(comm_size * limit) + ")"<< std::endl;
    while(std::stod(pfs_redis->subscribe_last().back().second.back().second) - start_ssd < 80 * comm_size * limit){
        write(file_ssd, buffer.c_str(), buffer.length());
    }
    ssd_timer.endTimeWithPrint("Id " + std::to_string(process_id) + "_" + std::to_string(thread_id) + ": ");
}

int main(int argc, char*argv[]){
    int provided;
//    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Init(&argc, &argv);
    int comm_size, id;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    std::string buffer = client_gen_random(16*1024*1024);

    std::string remote_host;
    int num_threads;
    if(argc == 3) {
        remote_host = argv[1];
        num_threads = atoi(argv[2]);
    }
    else exit(1);

    std::cout << remote_host << " " << comm_size << " " << id << std::endl;
    std::shared_ptr<redis_client> redis_memory, redis_nvme, pfs_redis;

    //    config.topic_+"_LDMS"
    redis_memory = std::make_shared<redis_client>("tcp://localhost", "MEMORY_"+ std::to_string(id));
    redis_nvme = std::make_shared<redis_client>("tcp://localhost", "NVME_"+ std::to_string(id));
    pfs_redis = std::make_shared<redis_client>(remote_host, "PFS_CAP");

    std::vector<std::thread> list_threads;
    list_threads.reserve(num_threads);

    MPI_Barrier(MPI_COMM_WORLD);

    Timer pop_timer;
    pop_timer.startTime();

    for(int i = 0; i < num_threads; i++){
        list_threads.emplace_back(std::thread(&do_io, redis_memory, redis_nvme, pfs_redis,
                                              buffer, id, i, comm_size));
    }

    for(std::thread &thread: list_threads){
        thread.join();
    }

    pop_timer.endTimeWithPrint("Real Test ");

    MPI_Finalize();
    return 0;
}

