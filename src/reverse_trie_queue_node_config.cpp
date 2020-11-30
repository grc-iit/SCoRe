//
// Created by neeraj on 4/27/20.
//
#include "reverse_trie_queue_node.h"
#include "config_generator.h"

ReverseTrieQueueNodeConfig ReverseTrieQueueNode::LoadConfiguration(std::string cluster_conf, int node_index) {
	/**
	 * Load file from json (std::unordered_map<int,ReverseTrieQueueNodeConfig>)
	 * and build the config (select the correct node_index)
	 */
    AUTO_TRACER("ReverseTrieQueueNode:LoadConfiguration");
	// TODO this here
	auto jason_map = conf::config_mapper(cluster_conf);
	auto rtq_config = conf::map_to_ReverseTriequeueNodeConfig(node_index, jason_map);
	return rtq_config;
}

ReverseTrieQueueNode::~ReverseTrieQueueNode() {
	Stop();
}

