//
// Created by neeraj on 4/17/20.
//

//
// Created by neeraj on 4/17/20.
//
#include "../../../include/common/queue/redis.h"
#include<iostream>
//#include "src/reverse_triequeue.h"



int main(){
	auto t = redis_client("tcp://127.0.0.1", "new_test");
	d_dict val;
	val.insert(std::make_pair <std::string ,std::string> (std::to_string(std::time(nullptr)), std::to_string(std::time(nullptr))+"we we will rock"));
	auto id = t.publish(val);
	std::cout << id << ' ' << t.latest_published_id<< ' ' <<t.latest_sub_id << std::endl;

	while (!t.sub_sync){

		auto p = t.subscribe_next();
		std::cout<< p.first << ' ' ;
		for (auto k : p.second){
			std::cout << ' ' << k.first << ' ' << k.second << std::endl;
		}

//		std::cout<<" NEXT ID: " << t.latest_sub_id<<std::endl;
	}

	std::cout << id << ' ' << t.latest_published_id<< ' ' <<t.latest_sub_id << std::endl;


	for (auto i: t.subscribe_all()){
		for (auto j : i.second){
			std::cout<<j.first<<' '<<j.second<<std::endl;
		}
	}

//	t.subscribe_all();
	return 0;
}
