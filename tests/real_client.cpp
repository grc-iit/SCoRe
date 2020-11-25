#include <iostream>
#include <unistd.h>
#include <timer.h>
#include <thread>
#include <redis.h>
#include <fcntl.h>
#include <mpi.h>
#include <future>

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

std::vector<uint64_t> measure(const std::shared_ptr<redis_client>& redis_memory,
                              const std::shared_ptr<redis_client>& redis_nvme,
                              const std::shared_ptr<redis_client>& pfs_redis,
                              int loops, double* counter){
    Timer timer;
    std::vector<uint64_t> measurements;

    timer.startTime();
    for(int i = 0; i < loops; i++) {
        measurements[0] = std::stod(redis_memory->subscribe_last().back().second.back().second);
        measurements[1] = std::stod(redis_nvme->subscribe_last().back().second.back().second);
        measurements[2] = std::stod(pfs_redis->subscribe_last().back().second.back().second);
    }
    *counter += timer.stopTime();

    return measurements;
}

void do_io(const std::shared_ptr<redis_client>& redis_memory,
           const std::shared_ptr<redis_client>& redis_nvme,
           const std::shared_ptr<redis_client>& pfs_redis,
           const std::string& buffer, int thread_id, uint64_t start_memory, uint64_t start_nvme,
           int num_loops,std::promise<double> p){
    uint64_t gig = (uint64_t)1*1024*1024*1024;
    uint64_t memroy_limit = 20 * gig;
    uint64_t nvme_limit = 40 * gig;

    double local_counter;

    std::string memory_path = "/mnt/nvme/jcernudagarcia/tempfs/test_mem_" + std::to_string(thread_id);
    std::string nvme_path = "/mnt/nvme/jcernudagarcia/apollo_nvme/test_nvme_" + std::to_string(thread_id);
    int file_memory = open(memory_path.c_str(), O_RDWR | O_CREAT, 0644);
    int file_nvme = open(nvme_path.c_str(), O_RDWR | O_CREAT, 0644);

    std::vector<uint64_t> measurements;

    do{
        write(file_memory, buffer.c_str(), buffer.length());
        measurements = measure(redis_memory, redis_nvme, pfs_redis, num_loops, &local_counter);
    }
    while(measurements[0] - start_memory < memroy_limit);

    do{
        write(file_nvme, buffer.c_str(), buffer.length());
        measurements = measure(redis_memory, redis_nvme, pfs_redis, num_loops, &local_counter);
    }
    while(measurements[1] - start_nvme < nvme_limit);

    p.set_value(local_counter);
}

void do_ssd(const std::shared_ptr<redis_client>& redis_memory,
            const std::shared_ptr<redis_client>& redis_nvme,
            const std::shared_ptr<redis_client>& pfs_redis, const std::string& buffer,
            int process_id, int thread_id, int comm_size, uint64_t start_ssd,
            int num_loops, std::promise<double> p){
    uint64_t gig = (uint64_t)1*1024*1024*1024;
    uint64_t limit = 35 * comm_size * gig;

    double pfs_counter;

    std::string ssd_path = "/mnt/nvme/jcernudagarcia/pvfs2-mount/test_ssd_" + std::to_string(process_id) + "_"+ std::to_string(thread_id);
    int file_ssd = open(ssd_path.c_str(), O_RDWR | O_CREAT, 0644);

    std::vector<uint64_t> measurements;

    do{
        write(file_ssd, buffer.c_str(), buffer.length());
        measurements = measure(redis_memory, redis_nvme, pfs_redis, num_loops, &pfs_counter);
    }
    while(measurements[2] - start_ssd < limit);

    p.set_value(pfs_counter);
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
    int num_lopps;
    if(argc == 3) {
        remote_host = argv[1];
        num_threads = atoi(argv[2]);
        num_lopps = atoi(argv[3]);
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
    std::vector<std::future<double>> local_return;
    std::vector<std::future<double>> ssd_return;
    list_threads.reserve(num_threads);
    ssd_threads.reserve(num_threads);
    local_return.reserve(num_threads);
    ssd_return.reserve(num_threads);

//    MPI_Barrier(MPI_COMM_WORLD);
    uint64_t start_ssd;
    auto start_memory = std::stoull(redis_memory->subscribe_last().back().second.back().second);
    auto start_nvme = std::stoull(redis_nvme->subscribe_last().back().second.back().second);
    if(comm_size == 1 || id == 0){
        start_ssd = std::stoull(pfs_redis->subscribe_last().back().second.back().second);
        std::cout << std::to_string(start_ssd) + "-" + std::to_string(start_ssd+(35 * comm_size * limit)) << std::endl;
    }

    Timer full_timer;
    full_timer.startTime();

    for(int i = 0; i < num_threads; i++){
        std::promise<double> p;
        local_return.push_back(std::move(p.get_future()));
        list_threads.emplace_back(std::thread(&do_io, redis_memory, redis_nvme, pfs_redis,
                                              buffer, i, start_memory, start_nvme,
                                              num_lopps, std::move(p)));
    }
    for(std::thread &thread: list_threads){
        thread.join();
    }

    if(comm_size != 1){
        MPI_Bcast(&start_ssd, 1, MPI_INT64_T, 0, MPI_COMM_WORLD);
    }
    for(int i = 0; i < num_threads; i++){
        std::promise<double> p;
        ssd_return.push_back(std::move(p.get_future()));
        ssd_threads.emplace_back(std::thread(&do_ssd, redis_memory, redis_nvme, pfs_redis,
                                             buffer, id, i, comm_size, start_ssd,
                                             num_lopps, std::move(p)));
    }
    for(std::thread &thread: ssd_threads){
        thread.join();
    }

    double final = full_timer.stopTime();

    double local_value = 0;
    double ssd_value = 0;
    for(auto & it : local_return) {
        local_value += it.get();
    }
    for(auto & it : ssd_return) {
        ssd_value += it.get();
    }
    double total_redis = (local_value + ssd_value)/num_threads;
    std::cout << "End_of_Process: " << id << " ;Total_Time: " << final << " ;Redis Time: " << total_redis << " ;Ratio: " << total_redis/final << std::endl;

    MPI_Finalize();
    return 0;
}