#pragma once

#ifndef SCORE_REVERSE_TRIE_QUEUE_H
#define SCORE_REVERSE_TRIE_QUEUE_H

#include<future>
#include<unordered_map>
#include <thread>
#include <chrono>
//#include "mon_hooks.h"
//#include"s_queues.h"
#include "reverse_trie_queue_node.h"

class ReverseTrieQueue {
public:
	std::shared_ptr<ReverseTrieQueueNode> root= nullptr;
	ReverseTrieQueue(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list, int root_index);
};



#endif