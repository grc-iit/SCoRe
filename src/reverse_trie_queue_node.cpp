//
// Created by neeraj on 4/26/20.
//

#include "reverse_trie_queue_node.h"

ReverseTrieQueueNode::ReverseTrieQueueNode(ReverseTrieQueueNodeConfig conf, Mode mode) : children() {
	// Constructor
	this->key = ReverseTrieQueueNodeKey(conf.key_);
	this->hostname_ = conf.hostname_;
	this->children = conf.children_;
	for (auto &i : conf.QueueConfigs_) {
		add(i);
	}
	if (mode == Mode::SERVER)
		async_loop();
}

int64_t ReverseTrieQueueNode::PopulateInterval(std::string val) {
    if (adapt_) {
        double curr_val = std::atof(val.data());
        if (curr_val < last_val_*(1+fluctuation_percentage_) && curr_val > last_val_ * (1 - fluctuation_percentage_)) {
            last_val_ = curr_val;
            return populate_interval_ += 100;
        }
        else {
            last_val_ = curr_val;
            return populate_interval_ -= 100;
        }
    }
    else {
        adapt_ = true;
        return populate_interval_;
    }
}

int64_t ReverseTrieQueueNode::PythioInterval(std::string val) {
    return pythio_interval_;
}

std::shared_ptr<queue> ReverseTrieQueueNode::add(QueueConfig config) {
	// add a queue to the RTQN based on the queueconfig
	std::shared_ptr<queue> t_queue;

	if (config.key_.type_.value == QueueValue::NODE_LOAD) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value == QueueValue::NODE_AVAILABILITY) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value == QueueValue::NODE_CAPACITY) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value == QueueValue::TIER_CAPACITY) {
		t_queue = std::make_shared<capacity_queue>(config);

	} else if (config.key_.type_.value == QueueValue::TIER_AVAILABILITY) {
		t_queue = std::make_shared<availability_queue>(config);

	} else if (config.key_.type_.value == QueueValue::TIER_CAPACITY) {
		t_queue = std::make_shared<capacity_queue>(config);

	} else if (config.key_.type_.value == QueueValue::CLUSTER_LOAD) {
		t_queue = std::make_shared<load_queue>(config);

	} else if (config.key_.type_.value == QueueValue::CLUSTER_AVAILABILITY) {
		t_queue = std::make_shared<availability_queue>(config);

	} else if (config.key_.type_.value == QueueValue::CLUSTER_CAPACITY) {
		t_queue = std::make_shared<capacity_queue>(config);
	} else {
		queue t_queue(config);
	}

	QueueKey kei(config.key_);
	queue_map.insert({{kei, t_queue}});
	return t_queue;
}

void ReverseTrieQueueNode::Stop() {
	// Stop method
	for (auto &i : this->promises) {
		i.set_value();
	}
	for (auto &i : this->pool) {
		i.join();
	}
	this->promises.clear();
	this->pool.clear();
}

void ReverseTrieQueueNode::async_loop() {
	// a simple async loop that runs in an separate thread
	for (auto i : queue_map) {
		std::promise<void> exit_signal;
		std::future<void> future_obj = exit_signal.get_future();
		std::thread t_thread(&ReverseTrieQueueNode::single_loop, this, i, std::move(future_obj));
		promises.push_back(std::move(exit_signal));
		pool.push_back(std::move(t_thread));
	}
	for (auto &i : this->pool) {
		i.join();
	}
}

void ReverseTrieQueueNode::single_loop(std::pair<QueueKey, std::shared_ptr<queue>> obj, std::future<void> futureObj) {
	// a the single loop that runs in a thread async 
	// ensures that they are all in the same level
	std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue = {};
	for (QueueConfig i : children) {
        std::unordered_map<QueueKey, std::shared_ptr<queue>> single_child_queue;
        single_child_queue.insert(make_pair(i.key_, add(i)));
//		if(i.key_.tier_index_ == this->key.level_ )child_queue.push_back(single_child_queue);
		child_queue.push_back(single_child_queue);
	}
	// infinite loop till kill comes thru
	/*
	uv_loop_t *loop;
	loop = uv_default_loop();
	uv_timer_t timer_req;
	uv_timer_init(loop, &timer_req);
	uv_timer_cb populate_cb = [](uv_timer_t *handle){
    #ifdef BENCH_TIMER
        Timer single_loop_timer;
        single_loop_timer.startTime();
    #endif
         auto data = (std::pair<std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>> *)handle->data;
         auto obj = data->first;
         auto child_queue = data->second;
	    if (obj.second->mon_hook_ == NULL) {
            obj.second->populate(child_queue);
        }
	    else {
	        obj.second->populate();
	    }
	    int64_t new_repeat = PopulateInterval();
	    uv_timer_set_repeat(handle, new_repeat);
    #ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
    #endif
	};
    uv_timer_cb populate_pythio_cb = [](uv_timer_t *handle){
#ifdef BENCH_TIMER
        Timer single_loop_timer;
        single_loop_timer.startTime();
#endif
        auto data = (std::pair<std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>> *)handle->data;
        auto obj = data->first;
        auto child_queue = data->second;
        obj.second->populate_pythio();
        int64_t new_repeat = PythioInterval();
        // How much reduce or increase interval to get expected rate?
        uv_timer_set_repeat(handle, new_repeat);
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    };
    static std::pair<std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>> dat = std::make_pair(obj, child_queue);
    timer_req.data = &dat;
    uv_timer_start(&timer_req, populate_cb, 0, std::chrono::milliseconds(obj.first.type_.interval).count());
    uv_timer_start(&timer_req, populate_pythio_cb, 0, std::chrono::milliseconds(1000).count());
    uv_run(loop, UV_RUN_DEFAULT);
    */
	/* Single thread idea: Outer loop monitoring, inner loop pythio, monitor time defined as time of n pythios. */
	fluctuation_percentage_ = 0.1;
	adapt_ = false;
	pythio_ratio_ = 5;
	pythio_counter_ = 5;
	populate_interval_ = obj.first.type_.interval;
	pythio_interval_ = populate_interval_ / pythio_ratio_;
    while (futureObj.wait_for(std::chrono::microseconds(populate_interval_)) == std::future_status::timeout) {
#ifdef BENCH_TIMER
        Timer single_loop_timer;
			single_loop_timer.startTime();
#endif
        if (obj.second->mon_hook_ == NULL) {
            obj.second->populate(child_queue);
        } else {
            if (pythio_counter_ < pythio_ratio_) {
                pythio_counter_++;
                std::string val = obj.second->populate_pythio();
                populate_interval_ = PopulateInterval(val);
            }
            else {
                pythio_counter_ = 0;
                std::string val = obj.second->populate();
                pythio_interval_ = PythioInterval(val);
                pythio_ratio_ = populate_interval_ / pythio_interval_;
            }
        }
        // add logic to change the interval of lookup
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
			std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
			single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    }  // while loop
}

std::vector<std::shared_ptr<ReverseTrieQueueNode>>
ReverseTrieQueueNode::get_children(std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children_config) {
	// creates children based on the config added
	auto children = std::vector<std::shared_ptr<ReverseTrieQueueNode>>();
	for (auto child_config:children_config) {
		children.push_back(std::make_shared<ReverseTrieQueueNode>(*child_config));
	}
	return children;
}

int ReverseTrieQueueNode::del(QueueKey key) {
	queue_map.erase(key);
	return 1;
}

std::shared_ptr<queue> ReverseTrieQueueNode::get(QueueKey key) {
	return queue_map.find(key)->second;

}

ReverseTrieQueueNode::ReverseTrieQueueNode(int argc, char **argv) {
	std::string conf_name;
	int node_index = 0;
	if (argc < 2) conf_name = "../configs/config_1.json";
	else conf_name = argv[1];
	if (argc < 3) node_index = 0;
	else node_index = atoi(argv[2]);
	ReverseTrieQueueNodeConfig conf = LoadConfiguration(conf_name, node_index);
	ReverseTrieQueueNode(conf, Mode::SERVER);
}

void ReverseTrieQueueNode::add_child(QueueConfig child) {
	children.push_back(child);
}
