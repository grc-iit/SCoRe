//
// Created by neeraj on 4/24/20.
//
#pragma once
#ifndef SCORE_CONFIG_GENERATOR_H
#define SCORE_CONFIG_GENERATOR_H

#include "reverse_trie_queue_node.h"
#include "reverse_trie_queue_node_config.h"
#include <fstream>
#include "nlohmann/json.hpp"



// keeps breaking if moved.
namespace conf {
	using json = nlohmann::json;
	std::string strip(std::string s);
	QueueType json_to_QueueType(json queue_type_json);
	QueueKey json_to_QueueKey(json queue_key_json);
	QueueConfig json_to_QueueConfig(json queue_config);
	ReverseTrieQueueNodeKey json_to_ReverseTriequeueNodeKey(json rtq_key_json);
	ReverseTrieQueueNodeConfig map_to_ReverseTriequeueNodeConfig(int me, std::unordered_map<int, json> index_to_json_map);
	auto map_all_to_ReverseTriequeueNodeConfig(std::unordered_map<int, json> config_maps);
	std::unordered_map<int, json> config_mapper(std::string config_filename);
    std::string topic_to_redis(json config, std::string topic);
    std::string topic_to_redis(std::unordered_map<int, conf::json> config, std::string topic);
}

#endif //SCORE_CONFIG_GENERATOR_H
