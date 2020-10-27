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


#include "common/queue/queue_config.h"
#include "common/queue/queue.h"

//#include "timer.h"


queue::queue(QueueConfig config) {
	// initializes queue based on config
	// config sets the key 
	if (this->key.mode_ == Mode::SERVER) { // clean up topic_ logic
	}
	redis = std::make_shared<redis_client>(config.url_, config.topic_);
	key = config.key_;
	lat_pub_id = "1"; // just an initial value
	lat_sub_id = "-"; // - implies from the dawn of time.
	mon_hook = config.hook_;

}

queue::queue(const queue &pQueue) {
	// copy constructor
	key = pQueue.key;
	lat_pub_id = pQueue.lat_pub_id;
	lat_sub_id = pQueue.lat_sub_id;
	lat_published = pQueue.lat_published;
	mon_hook = pQueue.mon_hook;

}

queue::queue(QueueKey kei, std::string url, std::string topic) {
	// copy constructor
	redis = std::make_shared<redis_client>(url, topic);
	key = kei;
	lat_pub_id = "";
	lat_sub_id = "";
	mon_hook = NULL;

}

std::string queue::publish(d_dict value) {
	// publishs a value to the queue
	lat_published = value;
	lat_pub_id = redis->publish(value);
	return lat_pub_id;
}

item_stream queue::subscribe() {
	// function to get all from the latest sub id
#ifdef BENCH_TIMER
	Timer sub_timer;
	sub_timer.startTime();
#endif
	item_stream result = redis->subscribe_all(lat_sub_id);
	lat_sub_id = result.back().first;
#ifdef BENCH_TIMER
	sub_timer.endTimeWithPrint("[Queue][Subscribe()]");
#endif
	return result;
}

d_dict queue::get_latest() {
	return lat_published;
}

bool queue::is_synced(std::vector<QM_type> children) {
	// checks if the queue is synced with all the other queues under it as per RTQ structure
	bool next = false;
	for (auto i : children) {
		for (auto j : i)
			next |= j.second->redis->sub_sync;   // <<---- this name isnt intuitive

		if (next)
			break;
	}
	return next;
}

std::string queue::populate() {
	// populates a value from the mon hook function. Fact collector
#ifdef BENCH_TIMER
	Timer pop_timer;
	pop_timer.startTime();
#endif

	d_dict val({{std::to_string(std::time(nullptr)), std::to_string(mon_hook())}});

#ifdef BENCH_TIMER
	pop_timer.endTimeWithPrint("[Queue][Populate()->mon_hook]");
#endif

	return publish(val);
}

std::string queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
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
	auto id = lat_pub_id;
	do {
		for (auto i: child_queue_maps) {
			for (auto j : i) {
				// << TODO Do optional type_ check  HERE
				auto k = j.second->redis->subscribe_next();
				if (k) {
					for (auto l : k->second) {
						second += std::stod(l.second);

					}
					val.insert({{std::to_string(std::time(nullptr)), std::to_string(second)}});
					lat_published = val;

					#ifdef BENCH_TIMER
						Timer pub_timer;
						pub_timer.startTime();
					#endif

					id = publish(val);

					#ifdef BENCH_TIMER
						pub_timer.endTimeWithPrint("[GenericQueue][Populate(vector)->Publish]");
					#endif
					lat_pub_id = id;

				} else {
					// what to do if the val is not there?

				}
			}
		}
	} while (!is_synced(child_queue_maps));
#ifdef BENCH_TIMER
	pop_timer.endTimeWithPrint("[Queue][Populate(vector)]");
#endif
	return id;
}
