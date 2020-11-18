//
// Created by neeraj on 4/26/20.
//

#include "reverse_trie_queue_node.h"

ReverseTrieQueueNode::ReverseTrieQueueNode(ReverseTrieQueueNodeConfig conf, Mode mode) : children() {
	// Constructor
    AUTO_TRACER("ReverseTrieQueueNode:Init");
	this->key = ReverseTrieQueueNodeKey(conf.key_);
	this->hostname_ = conf.hostname_;
	this->children = conf.children_;
	for (auto &i : conf.QueueConfigs_) {
		add(i);
	}
	if (mode == Mode::SERVER)
		async_loop();
}

int64_t ReverseTrieQueueNode::PopulateInterval(double last_val, double curr_val) {
    // TODO configurable window size, additive and division parameters
    int64_t return_interval;
    double diff_val = abs(curr_val - last_val);
    if (differences_.empty()) {
        differences_.push(diff_val);
        differences_sum_ = diff_val;
        return_interval = populate_interval_;
    }
    else {
        if (diff_val < (differences_sum_ / differences_.size()) * fluctuation_percentage_) {
            if (populate_interval_ + 1000000 > 25000000) {
                // Later, can configure max delay time
                return_interval = populate_interval_;
            }
            else {
                return_interval = populate_interval_ + 1000000;
            }
        }
        else {
            if ((populate_interval_ * 0.75) > MIN_DELAY_TIME) {
                return_interval = populate_interval_ * 0.75;
            }
            else {
                return_interval = populate_interval_;
            }
        }

        if (differences_.size() == 10) {
            differences_sum_ -= differences_.front();
            differences_.pop();
        }
        differences_.push(diff_val);
        differences_sum_ += diff_val;
    }
    return return_interval;
}

std::shared_ptr<queue> ReverseTrieQueueNode::add(QueueConfig config) {
    AUTO_TRACER("ReverseTrieQueueNode:SpawnQueue");
	// add a queue to the RTQN based on the queueconfig
	std::shared_ptr<queue> t_queue;

	if (config.key_.type_.value_ == QueueValue::NODE_LOAD) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::NODE_AVAILABILITY) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::NODE_CAPACITY) {
		t_queue = std::make_shared<mon_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::TIER_CAPACITY) {
		t_queue = std::make_shared<capacity_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::TIER_AVAILABILITY) {
		t_queue = std::make_shared<availability_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::TIER_LOAD) {
		t_queue = std::make_shared<load_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_LOAD) {
		t_queue = std::make_shared<load_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_AVAILABILITY) {
		t_queue = std::make_shared<availability_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_CAPACITY) {
        t_queue = std::make_shared<capacity_queue>(config);
    } else if (config.key_.type_.value_== QueueValue::SIM) {
	    t_queue = std::make_shared<sim_queue>(config);
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
        i.mode_=Mode::CLIENT;
        single_child_queue.insert(make_pair(i.key_, add(i)));
//		if(i.key_.tier_index_ == this->key.level_ )child_queue.push_back(single_child_queue);
		child_queue.push_back(single_child_queue);
	}
	// infinite loop till kill comes thru
    uv_loop_t *loop_measure = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_t *loop_predict = (uv_loop_t *)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop_measure);
    uv_loop_init(loop_predict);
    uv_timer_t timer_req_measure;
    uv_timer_t timer_req_predict;
    uv_timer_init(loop_measure, &timer_req_measure);
    uv_timer_init(loop_predict, &timer_req_predict);
    uv_timer_cb populate_cb = [](uv_timer_t *handle){
#ifdef BENCH_TIMER
        Timer single_loop_timer;
        single_loop_timer.startTime();
#endif
        auto data = (std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>, uv_timer_t *> *)handle->data;
        auto rtqn = std::get<0>(*data);
        auto obj = std::get<1>(*data);
        auto child_queue = std::get<2>(*data);
        auto handle2 = std::get<3>(*data);
        std::string val;
        /*
        auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        std::cout.rdbuf(coutbuf);
         */
        if (obj.second->mon_hook_ == NULL) {
            val = obj.second->populate(child_queue);
        } else {
            val = obj.second->populate();
        }
        std::string real_val = obj.second->lat_published_[obj.second->lat_published_.size()-1][0].second;
        double curr_val = std::strtod(real_val.data(), NULL); // std::strtod(val.data(), NULL);
        if (rtqn->last_measured_ > -1) {
            // auto time_start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            rtqn->populate_interval_ = rtqn->PopulateInterval(rtqn->last_measured_, curr_val);
            // auto time_end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
            // std::cout << time_end - time_start << std::endl;
            // std::cout.rdbuf(coutbuf);
            uv_timer_set_repeat(handle, rtqn->populate_interval_ / 1000);
            uv_timer_set_repeat(handle2, rtqn->populate_interval_ / (1000 * rtqn->pythio_ratio_));
        }
        rtqn->last_measured_ = curr_val;
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
        auto data = (std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>> *)handle->data;
        auto rtqn = std::get<0>(*data);
        auto obj = std::get<1>(*data);
        // auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        // std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        // std::cout.rdbuf(coutbuf);
        std::string val = obj.second->populate_pythio();
        double curr_val = std::strtod(val.data(), NULL);
        rtqn->last_predicted_ = curr_val;
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    };
    fluctuation_percentage_ = obj.first.type_.increase_factor_;
    last_measured_ = -1;
    last_predicted_ = -1;
    bool use_pythio_p = true;
    if (obj.first.type_.pythio_interval_ == 0) {
        use_pythio_p = false;
    }
    else {
        pythio_ratio_ = obj.first.type_.base_interval_ / obj.first.type_.pythio_interval_;
        pythio_counter_ = pythio_ratio_;
    }
    populate_interval_ = std::chrono::microseconds(obj.first.type_.base_interval_).count();
    differences_sum_ = 0;
    differences_ = std::queue<double>();
    // outfile = new std::ofstream("/home/kbateman/timings-score-two-loop.txt");
    if (obj.second->mon_hook_ == NULL) {
        obj.second->populate(child_queue);
    } else {
        obj.second->populate();
    }
    std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>,
            std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>, uv_timer_t *> dat = std::make_tuple(this, obj, child_queue, &timer_req_predict);
    timer_req_measure.data = &dat;
    std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>> dat2 = std::make_tuple(this, obj);
    timer_req_predict.data = &dat2;
    uv_timer_start(&timer_req_measure, populate_cb, MIN_DELAY_TIME / 1000, populate_interval_ / 1000);
    if (use_pythio_p) {
        uv_timer_start(&timer_req_predict, populate_pythio_cb, MIN_DELAY_TIME / 1000, populate_interval_ / (pythio_ratio_ * 1000));
    }
    if (use_pythio_p) {
        while (true) {
            uv_run(loop_measure, UV_RUN_NOWAIT);
            uv_run(loop_predict, UV_RUN_NOWAIT);
        }

    }
    else {
        while (true) {
            uv_run(loop_measure, UV_RUN_NOWAIT);
        }
    }
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
	if (argc < 2) conf_name = "../configs/config_2_1.json";
	else conf_name = argv[1];
	if (argc < 3) node_index = 0;
	else node_index = atoi(argv[2]);
	ReverseTrieQueueNodeConfig conf = LoadConfiguration(conf_name, node_index);
	ReverseTrieQueueNode(conf, Mode::SERVER);
}

void ReverseTrieQueueNode::add_child(QueueConfig child) {
	children.push_back(child);
}
