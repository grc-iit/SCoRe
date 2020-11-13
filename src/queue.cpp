//
// Created by neeraj on 4/26/20.
//
/*
 * Benchmarking notes
 * All measurement POIs should have an ID and also Specify the module it is running from.
 * so here in the populate
 * [Queue][Populate()][time] ->
 * [Queue][Populate(vector)][time] ->
 * [Queue][Populate(vector)->Publish][time] -> would mean the publish time in the publish of the populate(vector)
 *  if more than one type of vector, then specify further
 *  - give most least amount of info to ID
 *
 * */


#include <future>
#include "queue_config.h"
#include "queue.h"

//#include "timer.h"


std::string queue::publish(d_dict value) {
    AUTO_TRACER("Queue:Publish");
    lat_pub_id_ = redis_->publish(value);
	lat_published_.push_back(value);
	if(lat_published_.size() > window_size) lat_published_.erase (lat_published_.begin()+(lat_published_.size()-window_size));
	return lat_pub_id_;
}

void queue::subscribe_thread(std::promise<std::pair<std::string, std::string>> && p){
    item_stream return_value = subscribe();
    p.set_value(return_value.back().second.back());
}

item_stream queue::subscribe() {
    AUTO_TRACER("Queue:Subscribe");
	// function to get all from the latest sub id
#ifdef BENCH_TIMER
	Timer sub_timer;
	sub_timer.startTime();
#endif
	item_stream result = redis_->subscribe_last();
    if(!result.empty()) lat_sub_id_ = result.back().first;
#ifdef BENCH_TIMER
	sub_timer.endTimeWithPrint("[Queue][Subscribe()]");
#endif
	return result;
}

d_dict queue::get_latest() {
    AUTO_TRACER("Queue:get_latest");
	return lat_published_.back();
}

bool queue::is_synced(std::vector<QM_type> children) {
    AUTO_TRACER("Queue:is_synced");
	// checks if the queue is synced with all the other queues under it as per RTQ structure
	bool next = false;
	for (auto i : children) {
		for (auto j : i)
			next |= j.second->redis_->sub_sync;   // <<---- this name isnt intuitive

		if (next)
			break;
	}
	return next;
}

std::string queue::populate() {
    AUTO_TRACER("Queue:populate_monhook");
	// populates a value from the mon hook function. Fact collector
    #ifdef BENCH_TIMER
        Timer pop_timer;
        pop_timer.startTime();
    #endif
        d_dict val;
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    val.push_back({std::to_string(now), std::to_string(mon_hook_())});
    #ifdef BENCH_TIMER
        pop_timer.endTimeWithPrint("[Queue][Populate()->mon_hook]");
    #endif
	return publish(val);
}

std::string queue::populate_pythio() {
    AUTO_TRACER("Queue:populate_pythio");
    // populates a value from the mon hook function. Fact collector
    #ifdef BENCH_TIMER
        Timer pop_timer;
        pop_timer.startTime();
    #endif
    d_dict val(pythio_.Predict(lat_published_));
    #ifdef BENCH_TIMER
        pop_timer.endTimeWithPrint("[Queue][Populate()->mon_hook]");
    #endif
    return publish(val);
}

//std::string queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
//	// if populate has params, its a knowledge curator
//	// Takes queues from the vector of child queue maps and populates from there in a breadth first style
//	// These are generic queues, in s_queues.* you will see queues made to trace the REMCAP, LOAD and AVAIL at
//	// different tier leveles
//    #ifdef BENCH_TIMER
//        Timer pop_timer;
//        pop_timer.startTime();
//    #endif
//	d_dict val;
//	double second = 0;
//	auto id = lat_pub_id_;
//	do {
//		for (auto i: child_queue_maps) {
//			for (auto j : i) {
//				//TODO: Do optional type_ check  HERE
//                auto k = j.second->redis_->subscribe_next();
//                if (k) {
//                    auto l = k->second;
//                    second += std::stod(l.back().second);
//
//                    d_dict val;
//                    val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
//					#ifdef BENCH_TIMER
//						Timer pub_timer;
//						pub_timer.startTime();
//					#endif
//					id = publish(val);
//					#ifdef BENCH_TIMER
//						pub_timer.endTimeWithPrint("[GenericQueue][Populate(vector)->Publish]");
//					#endif
//					lat_pub_id_ = id;
//				} else {
//					// what to do if the val is not there?
//
//				}
//			}
//		}
//	} while (!is_synced(child_queue_maps));
//    #ifdef BENCH_TIMER
//        pop_timer.endTimeWithPrint("[Queue][Populate(vector)]");
//    #endif
//	return id;
//}

std::string queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
    AUTO_TRACER("Queue:populate_insight");
    // if populate has params, its a knowledge curator
    // Takes queues from the vector of child queue maps and populates from there in a breadth first style
    // These are generic queues, in s_queues.* you will see queues made to trace the REMCAP, LOAD and AVAIL at
    // different tier leveles
#ifdef BENCH_TIMER
    Timer pop_timer;
        pop_timer.startTime();
#endif
    d_dict val;
    double second = 0;
    auto id = lat_pub_id_;
    for (auto i: child_queue_maps) {
        for (auto j : i) {
            //TODO: Do optional type_ check  HERE
            auto k = j.second->redis_->subscribe_next();
            if (k) {
                auto l = k->second;
                second += std::stod(l.back().second);

                d_dict val;
                val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
                id = publish(val);
                lat_pub_id_ = id;
            } else {
                // what to do if the val is not there?

            }
        }
    }
    return id;
}

std::string gen_random(const int len) {
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

void queue::queue_publish_test(uint64_t num_repeats, uint64_t message_size) {
//[0] = {const char * | 0x56374be88a2a} "XADD"
//[1] = {const char * | 0x7fb694002f58} "CAPACITY_MEMORY"
//[2] = {const char * | 0x56374be8842a} "*"
//[3] = {const char * | 0x7fb68c001150} "1604776392"
//[4] = {const char * | 0x7fb68c001170} "8141456.000000"
//[4] = {unsigned long} 14
//[0] = {unsigned long} 4
//[1] = {unsigned long} 15
//[2] = {unsigned long} 1
//[3] = {unsigned long} 10
    d_dict val;
    val.push_back({"1604367412", gen_random(message_size)});
    for(int i = 0; i < num_repeats; i++){
        publish(val);
    }
}

void queue::queue_subscribe_test(uint64_t num_repeats) {
    for(int i = 0; i < num_repeats; i++){
        subscribe();
    }
}