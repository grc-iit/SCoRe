
#include "../include/redis.h"
#include<iostream>
//#include "src/reverse_triequeue.h"

#include <sw/redis++/redis++.h>
#include <redis.h>
using namespace sw::redis;


int main(){
    std::shared_ptr<redis_client> redisClient = std::make_shared<redis_client>("tcp://127.0.0.1", "new_test");
    d_dict attrs = { {"f1", "v1"}, {"f2", "v2"} };
    auto lat_pub_id_ = redisClient->publish(attrs);
    d_dict attrs1 = { {"f3", "v3"}, {"f4", "v4"} };
    lat_pub_id_ = redisClient->publish(attrs1);
    d_dict attrs2 = { {"f5", "v5"}, {"f6", "v6"} };
    lat_pub_id_ = redisClient->publish(attrs2);

    auto anwer = redisClient->subscribe_all();
    auto anwer2 = redisClient->subscribe_all();
	return  0;
}