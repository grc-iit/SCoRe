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
    return populate_interval_;
    /*
    if (curr_val < last_val*(1+fluctuation_percentage_) && curr_val > last_val * (1 - fluctuation_percentage_)) {
        return populate_interval_ += 100000;
    }
    else {
        if (populate_interval_ - 100000 > MIN_DELAY_TIME) {
            return populate_interval_ -= 100000;
        }
        else {
            return populate_interval_;
        }
    }
     */
}

int64_t ReverseTrieQueueNode::PythioInterval(double last_val, double curr_val) {
    if (curr_val < last_val*(1+fluctuation_percentage_) && curr_val > last_val * (1 - fluctuation_percentage_)) {
        return pythio_ratio_ += 0.8;
    }
    else {
        if (pythio_ratio_ > 0.8) {
            return pythio_ratio_ -= 0.8;
        }
        else {
            return pythio_ratio_;
        }
    }
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

	} else if (config.key_.type_.value_== QueueValue::TIER_CAPACITY) {
		t_queue = std::make_shared<capacity_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_LOAD) {
		t_queue = std::make_shared<load_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_AVAILABILITY) {
		t_queue = std::make_shared<availability_queue>(config);

	} else if (config.key_.type_.value_== QueueValue::CLUSTER_CAPACITY) {
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
        i.mode_=Mode::CLIENT;
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
        auto data = (std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>> *)handle->data;
        auto rtqn = std::get<0>(*data);
        auto obj = std::get<1>(*data);
        auto child_queue = std::get<2>(*data);
        auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        std::cout << "ts: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        std::cout.rdbuf(coutbuf);
        if (rtqn->pythio_counter_ < rtqn->pythio_ratio_) {
            rtqn->pythio_counter_++;
            std::string val = obj.second->populate_pythio();
            double curr_val = std::strtod(val.data(), NULL);
            rtqn->last_predicted_ = curr_val;
        }
        else {
            rtqn->pythio_counter_ = 0;
            std::string val;
            if (obj.second->mon_hook_ == NULL) {
                val = obj.second->populate(child_queue);
            } else {
                val = obj.second->populate();
            }
            double curr_val = std::strtod(val.data(), NULL);
            if (rtqn->last_measured_ > -1) {
                rtqn->populate_interval_ = rtqn->PopulateInterval(rtqn->last_measured_, curr_val);
                uv_timer_set_repeat(handle, rtqn->populate_interval_ / 1000);
            }
            rtqn->last_measured_ = curr_val;
        }
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    };
    fluctuation_percentage_ = 0.1;
    last_measured_ = -1;
    last_predicted_ = -1;
    pythio_ratio_ = 3;
    pythio_counter_ = 3;
    populate_interval_ = std::chrono::microseconds(obj.first.type_.base_interval_).count();
    outfile = new std::ofstream("/home/kbateman/timings-score-one-event.txt");
    if (obj.second->mon_hook_ == NULL) {
        obj.second->populate(child_queue);
    } else {
        obj.second->populate();
    }
    std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>,  std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>> dat = std::make_tuple(this, obj, child_queue);
    timer_req.data = &dat;
    uv_timer_start(&timer_req, populate_cb, MIN_DELAY_TIME / 1000, populate_interval_ / 1000);
    uv_run(loop, UV_RUN_DEFAULT);
    */
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
        //auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        //std::cout << "meas: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        //std::cout.rdbuf(coutbuf);
        if (obj.second->mon_hook_ == NULL) {
            val = obj.second->populate(child_queue);
        } else {
            val = obj.second->populate();
        }
        double curr_val = std::strtod(val.data(), NULL);
        if (rtqn->last_measured_ > -1) {
            rtqn->populate_interval_ = rtqn->PopulateInterval(rtqn->last_measured_, curr_val);
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
        //auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        //std::cout << "pred: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        //std::cout.rdbuf(coutbuf);
        std::string val = obj.second->populate_pythio();
        double curr_val = std::strtod(val.data(), NULL);
        rtqn->last_predicted_ = curr_val;
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    };
    fluctuation_percentage_ = 0.1;
    last_measured_ = -1;
    last_predicted_ = -1;
    pythio_ratio_ = 3;
    pythio_counter_ = 3;
    populate_interval_ = std::chrono::microseconds(obj.first.type_.base_interval_).count() * pythio_ratio_;
    //outfile = new std::ofstream("/home/kbateman/timings-score-two-loop.txt");
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
    uv_timer_start(&timer_req_predict, populate_pythio_cb, MIN_DELAY_TIME / 1000, populate_interval_ / (pythio_ratio_ * 1000));
    while (true) {
        uv_run(loop_measure, UV_RUN_NOWAIT);
        uv_run(loop_predict, UV_RUN_NOWAIT);
    }
	/*
	uv_loop_t *loop;
	loop = uv_default_loop();
	uv_timer_t timer_req;
	uv_timer_t timer_req2;
	uv_timer_init(loop, &timer_req);
	uv_timer_init(loop, &timer_req2);
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
        auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        std::cout << "meas: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        std::cout.rdbuf(coutbuf);
        if (obj.second->mon_hook_ == NULL) {
            val = obj.second->populate(child_queue);
        } else {
            val = obj.second->populate();
        }
        double curr_val = std::strtod(val.data(), NULL);
        if (rtqn->last_measured_ > -1) {
            rtqn->populate_interval_ = rtqn->PopulateInterval(rtqn->last_measured_, curr_val);
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
        auto coutbuf = std::cout.rdbuf(rtqn->outfile->rdbuf());
        std::cout << "pred: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
        std::cout.rdbuf(coutbuf);
        std::string val = obj.second->populate_pythio();
        double curr_val = std::strtod(val.data(), NULL);
        rtqn->last_predicted_ = curr_val;
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
        std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
	    single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    };
    fluctuation_percentage_ = 0.1;
    last_measured_ = -1;
    last_predicted_ = -1;
    pythio_ratio_ = 3;
    pythio_counter_ = 3;
    populate_interval_ = std::chrono::microseconds(obj.first.type_.base_interval_).count() * pythio_ratio_;
    outfile = new std::ofstream("/home/kbateman/timings-score-two-event.txt");
    if (obj.second->mon_hook_ == NULL) {
        obj.second->populate(child_queue);
    } else {
        obj.second->populate();
    }
    std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>,
            std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>>, uv_timer_t *> dat = std::make_tuple(this, obj, child_queue, &timer_req2);
    timer_req.data = &dat;
    std::tuple<ReverseTrieQueueNode *, std::pair<QueueKey, std::shared_ptr<queue>>> dat2 = std::make_tuple(this, obj);
    timer_req2.data = &dat2;
    uv_timer_start(&timer_req, populate_cb, MIN_DELAY_TIME / 1000, populate_interval_ / 1000);
    uv_timer_start(&timer_req2, populate_pythio_cb, MIN_DELAY_TIME / 1000, populate_interval_ / (pythio_ratio_ * 1000));
    uv_run(loop, UV_RUN_DEFAULT);
    */
	/* Single thread idea: Outer loop monitoring, inner loop pythio, monitor time defined as time of n pythios. */
	/*
	fluctuation_percentage_ = 0.1;
	last_measured_ = -1;
	last_predicted_ = -1;
	pythio_ratio_ = 3;
	pythio_counter_ = 3;
	populate_interval_ = obj.first.type_.base_interval_;
    while (futureObj.wait_for(std::chrono::microseconds(populate_interval_)) == std::future_status::timeout) {
#ifdef BENCH_TIMER
        Timer single_loop_timer;
			single_loop_timer.startTime();
#endif
        if (pythio_counter_ < pythio_ratio_) {
            pythio_counter_++;
            std::string val = obj.second->populate_pythio();
            double curr_val = std::strtod(val.data(), NULL);
            last_predicted_ = curr_val;
        }
        else {
            pythio_counter_ = 0;
            std::string val;
            if (obj.second->mon_hook_ == NULL) {
                val = obj.second->populate(child_queue);
            } else {
                val = obj.second->populate();
            }
            double curr_val = std::strtod(val.data(), NULL);
            if (last_measured_ > -1) {
                populate_interval_ = PopulateInterval(last_measured_, curr_val);
            }
            last_measured_ = curr_val;
        }
        // add logic to change the interval of lookup
#ifdef BENCH_TIMER
        // some ugly formatting here [RTQN][single loop(pair)]2[0.000142] < -- the 2 is off see?
			std::string prt_str = "[RTQN][single loop(pair)]" + std::to_string(obj.second->mon_hook == NULL? -1 : child_queue.size());
			single_loop_timer.endTimeWithPrint(prt_str.c_str());
#endif
    }  // while loop
    */
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
