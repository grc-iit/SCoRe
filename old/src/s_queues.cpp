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

#ifdef BENCH_TIMER
	Timer pop_timer;
	pop_timer.startTime();
#endif
	d_dict val;
	bool second = 0;
	auto id = lat_pub_id;
	while (!is_synced(child_queue_maps)) {
		for (auto i: child_queue_maps) {
			for (auto j : i) {
				// << TODO Do optional type_ check  HERE
				auto k = j.second->redis->subscribe_next();
				if (k) {
					for (auto l : k->second) {
//							second += std::stod(l.second);
						// here the OR operation is done on the data
						second |= (bool) std::stoi(l.second);

					}
					val.insert({{std::to_string(std::time(nullptr)), std::to_string(second)}});



					#ifdef BENCH_TIMER
						Timer pub_timer;
						pub_timer.startTime();
					#endif

					id = publish(val);

					#ifdef BENCH_TIMER
						pub_timer.endTimeWithPrint("[AvailabilityQueue][Populate(vector)->Publish]");
					#endif


				} else {
					// what to do if the val is not there?

				}
			}
		}
	}
#ifdef BENCH_TIMER
	pop_timer.endTimeWithPrint("[AvailabilityQueue][Populate(vector)]");
#endif
	return id;
}

std::string capacity_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {             // does addition

#ifdef BENCH_TIMER
	Timer pop_timer;
	pop_timer.startTime();
#endif

	d_dict val;
	double second = 0;
	auto id = lat_pub_id;
	while (!is_synced(child_queue_maps)) {
		for (auto i: child_queue_maps) {
			for (auto j : i) {
				// << TODO Do optional type_ check  HERE
				auto k = j.second->redis->subscribe_next();
				if (k) {
					for (auto l : k->second) {
						// Summation is done for the cacpacity
						second += std::stod(l.second);

					}

					val.insert({{std::to_string(std::time(nullptr)), std::to_string(second)}});
					#ifdef BENCH_TIMER
						Timer pub_timer;
						pub_timer.startTime();
					#endif

					id = publish(val);

					#ifdef BENCH_TIMER
						pub_timer.endTimeWithPrint("[CapacityQueue][Populate(vector)->Publish]");
					#endif


				} else {
					// what to do if the val is not there?

				}
			}
		}
	}
#ifdef BENCH_TIMER
	pop_timer.endTimeWithPrint("[CapacityQueue][Populate(vector)]");
#endif
	return id;
}

std::string load_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
	// does average
#ifdef BENCH_TIMER
	Timer pop_timer;
	pop_timer.startTime();
#endif
	d_dict val;
	double second = 0;
	auto id = lat_pub_id;
	while (!is_synced(child_queue_maps)) {
		for (auto i: child_queue_maps) {
			for (auto j : i) {
				// << TODO Do optional type_ check  HERE
				auto k = j.second->redis->subscribe_next();
				if (k) {
					for (auto l : k->second) {
						// Summation done here
						second += std::stod(l.second);
					}
					// divided by size done here to get average 
					val.insert({{std::to_string(std::time(nullptr)), std::to_string(second / k->second.size())}});


					#ifdef BENCH_TIMER
						Timer pub_timer;
						pub_timer.startTime();
					#endif

					id = publish(val);

					#ifdef BENCH_TIMER
						pub_timer.endTimeWithPrint("[LoadQueue][Populate(vector)->Publish]");
					#endif

				} else {
					// what to do if the val is not there?

				}
			}
		}
	}
#ifdef BENCH_TIMER
	pop_timer.endTimeWithPrint("[LoadQueue][Populate(vector)]");
#endif
	return id;
}
