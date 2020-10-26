
#include "../../../include/common/queue/redis.h"
#include<iostream>
//#include "src/reverse_triequeue.h"

#include <sw/redis++/redis++.h>
using namespace sw::redis;


int main(){
	auto t = redis_client("tcp://127.0.0.1", "new_test");
//	d_dict val;

	t.latest_sub_id = "1587352845893-0";
//	std::cout <<' ' << t.latest_published_id<< ' ' <<t.latest_sub_id << std::endl;


	std::vector<Optional<std::pair<std::string, item_stream>>> result;

	t.redis->xread(t.topic, t.latest_sub_id, 1, std::back_inserter(result));
	std::cout<<result.size();
	for (const auto i : result) {
		if (i) {

			std::cout<<"I1 : "<<i->first<<std::endl;
			for(auto j : i->second){
				std::cout <<"J1 : " << j.first<<std::endl;
				for(auto k: j.second){
					std::cout<<"K1 : " << k.first << " K2 : " <<k.second<<std::endl;
					}
				}
			}

		}


	/*	for (auto i: result){
			std::cout<<;

		}*/

	return  0;
}