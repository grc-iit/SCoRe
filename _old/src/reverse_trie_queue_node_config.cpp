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

	// TODO this here
	auto jason_map = conf::config_mapper(cluster_conf);
	auto rtq_config = conf::map_to_ReverseTriequeueNodeConfig(node_index, jason_map);
	return rtq_config;
}

ReverseTrieQueueNode::~ReverseTrieQueueNode() {
	Stop();
}

ReverseTrieQueueNodeConfig::ReverseTrieQueueNodeConfig() {
	ReverseTrieQueueNodeKey k;
	hostname_ = "localhost";
	key_ = k;
	me_ = 0;
	QueueKey qk;
	QueueConfig qc(qk, "tcp://localhost", "UNSET", Mode::SERVER);
	children_ = {};
	QueueConfigs_ = {qc};
}

ReverseTrieQueueNodeConfig::ReverseTrieQueueNodeConfig(ReverseTrieQueueNodeKey key, std::string hostname,
                                                       std::vector<QueueConfig> configs,
                                                       std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children, int me) {
	key_ = key;
	me_ = me;
	children_ = children;
	QueueConfigs_ = configs;
	hostname_ = hostname;

}
