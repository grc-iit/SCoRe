//
// Created by neeraj on 4/17/20.
//

#include "s_queues.h"
#include <iostream>
#include <unistd.h>
#include <mon_hooks.h>

int main(){
	d_dict val;
	unsigned int microseconds = 1000;
	auto url = "tcp://127.0.0.1";
	auto topic1 = "q_test1";

	QueueKey key(0, 0, QueueType(QueueValue::NODE_CAPACITY, 0.2), Mode::SERVER);

//    QueueKey key_;
//    std::string url_;
//    std::string topic_;
//    Mode mode_;
//    double_function hook_;
//    Model model_;
//    std::string weights_;
//    int queue_port_;

	QueueConfig config(key, url, topic1, Mode::SERVER, mon::cap_hook, Model::LINEAR, "", 3000);
	mon_queue qu1(config);

	qu1.populate();
	qu1.populate();
	qu1.populate();

	item_stream answers = qu1.subscribe();

	return 0;
}