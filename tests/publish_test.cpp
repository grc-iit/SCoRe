//
// Created by neeraj on 4/17/20.
//

#include "s_queues.h"
#include <iostream>
#include <unistd.h>
#include <mon_hooks.h>
#include <timer.h>
#include <thread>

int main(int argc, char*argv[]){
	auto url = "tcp://127.0.0.1";
	auto topic1 = "publish_test_";
    int num_queues = 16;
    int num_messages = 441505;

    if (argc < 3){
        std::cout << "Need num_queues and num_messages" << std::endl;
    }
    else {
        num_queues = atoi(argv[2]);
        num_messages = atoi(argv[2]);
    }

	std::vector<queue> list_queues;
	std::vector<std::thread> list_threads;
    list_threads.reserve(num_queues);
    list_queues.reserve(num_queues);

    for(int i = 0; i < num_queues; i++){
        QueueKey key(0, 0, QueueType(QueueValue::NODE_CAPACITY, 0.2), Mode::SERVER);
        QueueConfig config(key, url, topic1+std::to_string(i), Mode::SERVER, mon::cap_hook, Model::LINEAR, "", 3000);
        list_queues.emplace_back(queue(config));
    }

    Timer pop_timer;
    pop_timer.startTime();

    for(int i = 0; i < num_queues; i++){
        list_threads.emplace_back(std::thread(&queue::queue_test, list_queues[i], num_messages));
	}

    for(std::thread &thread: list_threads){
        thread.join();
    }

    pop_timer.endTimeWithPrint("Publish Test ");

	return 0;
}