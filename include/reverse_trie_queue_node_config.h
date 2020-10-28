//
// Created by neeraj on 4/27/20.
//

#ifndef SCORE_REVERSE_TRIE_QUEUE_NODE_CONFIG_H
#define SCORE_REVERSE_TRIE_QUEUE_NODE_CONFIG_H
#include "reverse_trie_queue_node_key.h"
#include "queue_config.h"

class ReverseTrieQueueNodeConfig {

public:
	ReverseTrieQueueNodeKey key_;
	int me_;
	std::vector<QueueConfig> QueueConfigs_;
	std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children_;
	std::string hostname_;

	ReverseTrieQueueNodeConfig(ReverseTrieQueueNodeKey key, std::string hostname , std::vector<QueueConfig> configs,
			std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children, int me);
	ReverseTrieQueueNodeConfig();
};



#endif //SCORE_REVERSE_TRIE_QUEUE_NODE_CONFIG_H
