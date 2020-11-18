//
// Created by neeraj on 4/26/20.
//

#include <thread>
#include <future>
#include <timer.h>
#include "s_queues.h"

// Here the whole procedure is similar to the queues.* populate methods
// maybe it can be altered to only have different final operations
// instead of collecting them and doing the ops on them on the fly, 
// maybe add them to a vector and have a do operation method where it precesses the
// data specifically for each qeueue type
//second |= (bool) std::stoi(fact);
std::string
sim_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {
    AUTO_TRACER("simulation_queue:populate_insight");
    d_dict val;
    bool second = 0;
    auto id = lat_pub_id_;
    for (const auto& queue_map: child_queue_maps) {
        for (const auto& queue_pair : queue_map) {
            // << TODO Do optional type_ check  HERE
            item_stream result = queue_pair.second->subscribe();
            auto fact = result.back().second.back().second;
            if (!fact.empty()) {
                // here the OR operation is done on the data
                second |= (bool) std::stoi(fact);
            } else {
                // what to do if the val is not there?
            }
        }
    }
    val.push_back({std::to_string(std::time(nullptr)), std::to_string(second)});
    id = publish(val);
    return id;
}

std::string
availability_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {             // does addition
	// find a way to do this till the latest queues some sync
    AUTO_TRACER("availability_queue:populate_insight");
	d_dict val;
	bool second = 0;
	auto id = lat_pub_id_;
    for (const auto& queue_map: child_queue_maps) {
        for (const auto& queue_pair : queue_map) {
            // << TODO Do optional type_ check  HERE
            item_stream result = queue_pair.second->subscribe();
            std::string fact;
            if(!result.empty()) fact = result.back().second.back().second;
            if (!fact.empty()) {
				// here the OR operation is done on the data
				second |= (bool) std::stoi(fact);
			} else {
				// what to do if the val is not there?
			}
		}
	}
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    val.push_back({std::to_string(now), std::to_string(second)});
	id = publish(val);
	return id;
}

std::string capacity_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps) {             // does addition
    AUTO_TRACER("capacity_queue:populate_insight");
    d_dict val;
	double second = 0;
	auto id = lat_pub_id_;
    std::vector<std::thread> list_threads;
    std::vector<std::future<std::pair<std::string, std::string>>> return_vector;

    for (const auto& queue_map: child_queue_maps) {
        for (const auto &queue_pair : queue_map) {
            // << TODO Do optional type_ check  HERE
            std::promise<std::pair<std::string, std::string>> p;
            return_vector.push_back(std::move(p.get_future()));
            list_threads.emplace_back(std::thread(&queue::subscribe_thread, queue_pair.second, std::move(p)));
        }
    }

    for(std::thread &thread: list_threads){
        thread.join();
    }

    #ifdef COMMON_DEBUG_TIMER
    Timer pop_timer;
    pop_timer.startTime();
    #endif
    for(auto & it : return_vector) {
//    for(auto facts: return_vector){
        auto fact = it.get().second;
        if (!fact.empty()) {
            second += std::stod(fact);
        }
    }

    #ifdef COMMON_DEBUG_TIMER
    pop_timer.endTimeWithPrint("Insight Calculations: ");
    #endif
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    val.push_back({std::to_string(now), std::to_string(second)});
    return publish(val);
}

//second += std::stod(fact);
std::string load_queue::populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps)  {
	// does average
    AUTO_TRACER("load_queue:populate_insight");
	d_dict val;
	double second = 0;
	auto id = lat_pub_id_;
    for (const auto& queue_map: child_queue_maps) {
        for (const auto& queue_pair : queue_map) {
            // << TODO Do optional type_ check  HERE
            item_stream result = queue_pair.second->subscribe();
            auto fact = result.back().second.back().second;
            if (!fact.empty()) {
				// Summation done here
				second += std::stod(fact);
			} else {
				return "0";
			}
		}
	}
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    val.push_back({std::to_string(now), std::to_string(second)});
	id = publish(val);
	return id;
}
