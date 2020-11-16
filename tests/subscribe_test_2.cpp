#include "s_queues.h"
#include <iostream>
#include <unistd.h>
#include <mon_hooks.h>
#include <timer.h>
#include <thread>
#include "mpi.h"

int main(int argc, char*argv[]){
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int start = 13;
    int num_nodes = 2;
    int num_queues = 1;
    auto url = "tcp://127.0.0.1";
    auto topic1 = "publish_test_0";
    int num_queues = 16;
    uint64_t num_messages = 441505;

    if (argc < 3){
        std::cout << "Need num_queues and num_messages" << std::endl;
        return 1;
    }
    else {
        url = argv[1];
        num_queues = atoi(argv[2]);
        num_messages = atoi(argv[3]);
    }

    std::vector<queue> list_queues;
    std::vector<std::thread> list_threads;
    list_threads.reserve(num_queues);
    list_queues.reserve(num_queues);

    for(int i = 0; i < num_queues; i++){
        QueueKey key(0, 0, QueueType(QueueValue::NODE_CAPACITY, 0,0,0), Mode::SERVER);
        QueueConfig config(key, url, "", topic1, Mode::SERVER, mon::cap_hook, Model::LINEAR, "", 3000);
        list_queues.emplace_back(queue(config));
    }

    Timer pop_timer;
    pop_timer.startTime();

    for(int i = 0; i < num_queues; i++){
        list_threads.emplace_back(std::thread(&queue::queue_subscribe_test, list_queues[i], num_messages));
    }

    for(std::thread &thread: list_threads){
        thread.join();
    }

    pop_timer.endTimeWithPrint("Subscribe Test " + std::to_string(world_rank) + " ");

    MPI_Finalize();
    return 0;
}
