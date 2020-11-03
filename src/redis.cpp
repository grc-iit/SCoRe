//
// Created by neeraj on 4/26/20.
//

#include <common/debug.h>
#include "redis.h"

redis_client::redis_client(std::string url, std::string topic) {
	// constructor
    AUTO_TRACER("Redis:Init");
	auto t = new sw::redis::Redis(url);
	this->topic = topic;
	this->redis = std::shared_ptr<sw::redis::Redis>(t);
	this->latest_pub_id = "1";
	this->latest_sub_id = "1";
	this->sub_sync = false;
//        last_item = {"NULL",{"NULL","NULL"}};
}

redis_client &redis_client::operator=(redis_client obj) {
	// copy constructor
	url = obj.url;
	redis = obj.redis;
	latest_entry = obj.latest_entry;
	latest_pub_id = obj.latest_pub_id;
	latest_sub_id = obj.latest_sub_id;
	this->sub_sync = obj.sub_sync;

	return *this;
}

item_stream redis_client::subscribe_all() {
	// gets everything from the last subscribed id
    AUTO_TRACER("Redis:subscribe_all_1");
	item_stream result;
	this->redis->xrange(topic, this->latest_sub_id, "+", std::back_inserter(result));
	this->latest_sub_id = result.back().first;
//        sub_sync = latest_published_id.compare(latest_sub_id) ==  0  ? true  : false;
	return result;
}

item_stream redis_client::subscribe_all(std::string ls_id) {
	// gets everything from ls_id (list id)
    AUTO_TRACER("Redis:subscribe_all_2");
	item_stream result;
	this->redis->xrange(topic, ls_id, "+", std::back_inserter(result));
	this->latest_sub_id = result.back().first;
//        sub_sync = latest_published_id.compare(latest_sub_id) ==  0  ? true  : false;
	return result;
}

std::string redis_client::publish(d_dict val) {
    AUTO_TRACER("Redis:publish");
	// publishs val to the queue, the topic is fixed per queue
	this->latest_entry = val;
	auto begin = val.begin();
	auto end = val.end();
	this->latest_pub_id = this->redis->xadd(topic, "*", begin, end);
	return latest_pub_id;
//    d_dict attrs = { {"f1", "v1"}, {"f2", "v2"} };
//	this->latest_pub_id = this->redis->xadd(topic, "*", attrs.begin(), attrs.end());
//	return latest_pub_id;

}

std::optional<item> redis_client::subscribe_next() { // << TODO change this to optional so that --- *
    AUTO_TRACER("Redis:subscribe_next");
	// gets the next entry in the queue. 
	std::vector<sw::redis::Optional<std::pair<std::string, item_stream>>> result;
	auto cur_id = this->latest_sub_id;
	std::optional<item> ret_val;
//	    redis.xread("key_", id, 10, std::inserter(result, result.end()));
	this->redis->xread(this->topic, this->latest_sub_id, 1, std::inserter(result, result.end()));

	if (result.size() > 0) {
		for (const auto i : result) {
			if (i) {
//					std::cout << "I1 : " << i->first << std::endl;
				for (auto j : i->second) {
//						std::cout << " OLD ID " << latest_sub_id << " NEW ID " << j.first <<std::endl;
					this->latest_sub_id = j.first;
					ret_val = j;
				}
			}
		}
	} else {
		ret_val = std::nullopt;  //  << we dont need to put in the last value for every check
		sub_sync = true;
	}
	return ret_val;
}
