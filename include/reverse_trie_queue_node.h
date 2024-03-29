//
// Created by neeraj on 4/18/20.
//
#pragma once

#ifndef SCORE_REVERSE_TRIE_QUEUE_NODE_H
#define SCORE_REVERSE_TRIE_QUEUE_NODE_H

#include <chrono>
#include <thread>
#include <future>
#include <tuple>
#include <queue>
#include "mon_hooks.h"
#include "s_queues.h"
#include "reverse_trie_queue_node_key.h"
#include "config_generator.h"
#include "reverse_trie_queue_node_config.h"
#include <uv.h>
#include <constants.h>

#ifdef BENCH_TIMER
#include "timer.h"
#endif


class ReverseTrieQueueNode {
private:
    std::pair<QueueKey, std::shared_ptr<queue>> curr_obj_;
    std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> curr_child_queue_;
    int64_t PopulateInterval(double last_val, double curr_val);
    int64_t PythioInterval(double last_val, double curr_val);
    int pythio_ratio_;
    int pythio_counter_;
    int64_t populate_interval_;
    double fluctuation_percentage_;
    double last_predicted_;
    double last_measured_;
    std::queue<double> differences_;
    double differences_sum_;
    // std::ofstream *outfile;
    bool use_pythio_p_;
    bool use_adaptivity_p_;
public:
	ReverseTrieQueueNodeKey key;
	QM_type queue_map;
	std::shared_ptr<ReverseTrieQueueNode> parent;
	std::vector<QueueConfig> children;
	std::vector<std::thread> pool;
	std::vector< std::promise<void> > promises;
	std::vector< std::future<void>> futures_;
	std::string hostname_;

	std::shared_ptr<queue> add(QueueConfig config);
	std::vector<std::shared_ptr<ReverseTrieQueueNode>> get_children(
			std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children_config);
	ReverseTrieQueueNodeConfig LoadConfiguration(std::string cluster_conf, int node_index);
	ReverseTrieQueueNode(int argc, char* argv[]);
	ReverseTrieQueueNode(ReverseTrieQueueNodeConfig conf, Mode mode = Mode::CLIENT);
	std::shared_ptr<queue> get(QueueKey key);
	void add_child(QueueConfig child);
	[[maybe_unused]] int del(QueueKey key);
	void single_loop(std::pair<QueueKey, std::shared_ptr<queue>> obj, std::future<void> futureObj);
	void async_loop();
	void Stop();
	~ReverseTrieQueueNode();

};

#endif //SCORE_REVERSE_TRIE_QUEUE_NODE_H
