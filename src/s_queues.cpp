//
// Created by neeraj on 4/26/20.
//

#include "s_queues.h"

// Here the whole procedure is similar to the queues.* populate methods
// maybe it can be altered to only have different final operations
// instead of collecting them and doing the ops on them on the fly, 
// maybe add them to a vector and have a do operation method where it precesses the
// data specifically for each qeueue type
std::string
availability_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {             // does addition
	// find a way to do this till the latest queues some sync
	d_dict val;
	bool second = 0;
	auto id = lat_pub_id_;
	for (auto i: child_queue_maps) {
		for (auto j : i) {
			// << TODO Do optional type_ check  HERE
			auto k = j.second->redis_->subscribe_next();
			if (k) {
				auto l = k->second;
				// here the OR operation is done on the data
				second |= (bool) std::stoi(l.back().second);
			} else {
				// what to do if the val is not there?
			}
		}
	}
	val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
	id = publish(val);
	return id;
}

std::string capacity_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {             // does addition
	d_dict val;
	double second = 0;
	auto id = lat_pub_id_;
	for (auto i: child_queue_maps) {
		for (auto j : i) {
			// << TODO Do optional type_ check  HERE
			auto k = j.second->redis_->subscribe_next();
			if (k) {
				auto l = k->second;
				// Summation is done for the cacpacity
				second += std::stod(l.back().second);
			} else {
				// what to do if the val is not there?
			}
		}
	}
	val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
	id = publish(val);
	return id;
}

std::string load_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
	// does average
	d_dict val;
	double second = 0;
	auto id = lat_pub_id_;
	for (auto i: child_queue_maps) {
		for (auto j : i) {
			// << TODO Do optional type_ check  HERE
			auto k = j.second->redis_->subscribe_next();
			if (k) {
				auto l = k->second;
				// Summation done here
				second += std::stod(l.back().second);
			} else {
				// what to do if the val is not there?
			}
		}
	}
	val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
	id = publish(val);
	return id;
}
