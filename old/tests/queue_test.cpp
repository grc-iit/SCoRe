//
// Created by neeraj on 4/17/20.
//

#include "../include/s_queues.h"
#include <iostream>
#include <unistd.h>
//#include "debug.h"
int te=0;
double cap(){
	return te*322.14;
}

int main(){
	d_dict val;
//	AutoTrace val_trace = AutoTrace("Queue Value",val);
	unsigned int microseconds = 1000;
	val.insert(std::make_pair <std::string ,std::string> ("this time", "we we will rock"));
	auto url = "tcp://127.0.0.1";
	auto topic1 = "q_test1";
	auto topic2 = "q_test2";

	QueueKey key(2, 3, QueueType(QueueValue::NODE_CAPACITY, 0.2), Mode::SERVER);
	QueueConfig config(key, url, topic1, Mode::SERVER, cap);
	mon_queue qu1(config);

//	qu1.redis->subscribe_all();
//	std::cout << "IDS " << qu1.lat_sub_id<< ' ' << qu1.lat_pub_id << std::endl;

	QueueKey key2(2, 3, QueueType(QueueValue::TIER_CAPACITY, 0.2), Mode::SERVER);
	QueueConfig config2(key, url, topic2, Mode::SERVER, cap);
	capacity_queue qu2(config2);

//	AutoTrace queue_trace = AutoTrace("Queue 1",qu1);

	qu1.populate();
	qu1.subscribe();
//	std::cout<<"populated\n";
//	std::cout << "IDS " << qu1.lat_sub_id<< ' ' << qu1.lat_pub_id << std::endl;
//
//	std::cout << ++te << " FIRST " << qu1.lat_pub_id << std::endl;
//
//	std::cout<< "reading\n";
/*	for (auto i: qu1.subscribe()){

		std::cout << i.first << ' ';
		for (auto j: i.second ){
			std::cout << j.first << ' ' << j.second << std::endl;
		}
	}*/

//	std::cout<<te << " and here we go\n" ;
	usleep(microseconds);
//	std::cout << "IDS " << qu1.lat_sub_id<< ' ' << qu1.lat_pub_id << std::endl;

	qu1.populate();
//	std::cout<<"populated\n";

	te++;
//	std::cout << "ReADINF";
	for (auto i: qu1.subscribe()){

		std::cout << i.first << ' ' ;
		for (auto j: i.second ){
			std::cout << j.first << ' ' << j.second << std::endl;
		}
	}

	te++;

//	std::cout<<te << "ANNNNNND here we go\n" ;
//	usleep(microseconds);
//
//	std::cout<<"NOT populated\n";
//
	te++;
	std::cout << "ReADING\n";

	std::cout << "IDS " << qu1.lat_sub_id<< ' ' << qu1.lat_pub_id << std::endl;

/*
	for (auto i: qu1.subscribe()){

		std::cout << i.first << ' ' ;
		for (auto j: i.second ){
//			std::cout << j.first << ' ' << j.second << std::endl;
		}
	}
*/

	return 0;
}