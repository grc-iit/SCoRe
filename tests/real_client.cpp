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
           const std::string& buffer, int thread_id, uint64_t start_memory, uint64_t start_nvme){
    uint64_t gig = (uint64_t)1*1024*1024*1024;
    uint64_t memroy_limit = 20 * gig;
    uint64_t nvme_limit = 40 * gig;

    std::string memory_path = "/mnt/nvme/jcernudagarcia/tempfs/test_mem_" + std::to_string(thread_id);
    std::string nvme_path = "/mnt/nvme/jcernudagarcia/apollo_nvme/test_nvme_" + std::to_string(thread_id);
    int file_memory = open(memory_path.c_str(), O_RDWR | O_CREAT, 0644);
    int file_nvme = open(nvme_path.c_str(), O_RDWR | O_CREAT, 0644);

    while(std::stod(redis_memory->subscribe_last().back().second.back().second) - start_memory < memroy_limit){
        write(file_memory, buffer.c_str(), buffer.length());
    }

    while(std::stod(redis_nvme->subscribe_last().back().second.back().second) - start_nvme < nvme_limit){
        write(file_nvme, buffer.c_str(), buffer.length());
    }
//    std::cout << "done NVMe" << std::endl;
}

void do_ssd(const std::shared_ptr<redis_client>& pfs_redis, const std::string& buffer,
           int process_id, int thread_id, int comm_size, uint64_t start_ssd){
    uint64_t gig = (uint64_t)1*1024*1024*1024;
    uint64_t limit = 70 * comm_size * gig;

    std::string ssd_path = "/mnt/nvme/jcernudagarcia/pvfs2-mount/test_ssd_" + std::to_string(process_id) + "_"+ std::to_string(thread_id);
    int file_ssd = open(ssd_path.c_str(), O_RDWR | O_CREAT, 0644);

    while(std::stod(pfs_redis->subscribe_last().back().second.back().second) - start_ssd < limit){
        write(file_ssd, buffer.c_str(), buffer.length());
    }
}


int main(int argc, char*argv[]){
    int provided;
//    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Init(&argc, &argv);
    int comm_size, id;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    std::string buffer = client_gen_random(16*1024*1024);
    uint64_t limit = (uint64_t)1*1024*1024*1024;

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
    std::vector<std::thread> ssd_threads;
    list_threads.reserve(num_threads);
    ssd_threads.reserve(num_threads);

//    MPI_Barrier(MPI_COMM_WORLD);
    uint64_t start_ssd;
    auto start_memory = std::stoull(redis_memory->subscribe_last().back().second.back().second);
    auto start_nvme = std::stoull(redis_nvme->subscribe_last().back().second.back().second);
    if(comm_size == 1 || id == 0){
        start_ssd = std::stoull(pfs_redis->subscribe_last().back().second.back().second);
        std::cout << std::to_string(start_ssd) + "-" + std::to_string(start_ssd+(70 * comm_size * limit)) << std::endl;
    }

    Timer pop_timer;
    pop_timer.startTime();

    for(int i = 0; i < num_threads; i++){
        list_threads.emplace_back(std::thread(&do_io, redis_memory, redis_nvme, buffer, i, start_memory, start_nvme));
    }

    for(std::thread &thread: list_threads){
        thread.join();
    }

    if(comm_size != 1){
        std::cout << id << " Entry" << std::endl;
        MPI_Bcast(&start_ssd, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
        std::cout << id << " Exit" << std::endl;
    }

    for(int i = 0; i < num_threads; i++){
        ssd_threads.emplace_back(std::thread(&do_ssd, pfs_redis, buffer, id, i, comm_size, start_ssd));
    }

    for(std::thread &thread: ssd_threads){
        thread.join();
    }

    pop_timer.endTimeWithPrint("Real Test ");

    MPI_Finalize();
    return 0;
}

