//
// Created by neeraj on 4/30/20.
//

#include <config_generator.h>
#include "SCoRe.h"

client::SCoRe::SCoRe(std::string config_path, std::string topic, int node) {
    auto jason_map = conf::config_mapper(config_path);
    std::string redis_url = conf::topic_to_redis(jason_map[node], topic);
    client_ = std::make_shared<redis_client>(redis_url, topic);
}

client::SCoRe::SCoRe(std::string config_path, std::string topic) {
    auto jason_map = conf::config_mapper(config_path);
    std::string redis_url = conf::topic_to_redis(jason_map, topic);
    client_ = std::make_shared<redis_client>(redis_url, topic);
}

std::pair<std::string, std::string> client::SCoRe::get_latest() {
	return client_->subscribe_all().back().second.back();
}

/********************************************************************************************************************/
//auto server::SCoRe::get_latest() {
//	SCoRe_return aggregate;
//	for (auto i:score->root->queue_map) {
////			auto key_ = i.first.type_.value;
//		auto val = i.second->lat_published_.back();
//		auto topic = i.second->redis_->topic;
//		aggregate.insert({{topic, val}});
//	}
//	return aggregate;
//}
//
//server::SCoRe::SCoRe(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list) {
//
//	score = std::make_shared<ReverseTrieQueue>(node_list, 0);
//	auto srv = new rpc::server(6666);
//	srv->bind("get_latest", [this](std::string s) { return get_latest(); });
//	this->server = srv;
//}
//
///*
// * get a list of nodes?
//ReverseTrieQueueNodeConfig server::SCoRe::LoadConfigurations(std::string cluster_conf, int node_index) {
//
//	auto jason_map = conf::config_mapper(cluster_conf);
//	if (node_index != 0) throw std::invalid_argument( "node index != 0 for root" );
//
//	auto rtq_config = conf::map_to_ReverseTriequeueNodeConfig(node_index, jason_map);
//	return rtq_config;
//}
//*/
//
//ReverseTrieQueueNodeConfig server::SCoRe::LoadConfiguration(std::string cluster_conf, int node_index) {
//	auto jason_map = conf::config_mapper(cluster_conf);
//	//if (node_index != 0) throw std::invalid_argument( "node index != 0 for root" );
//	auto rtq_config = conf::map_to_ReverseTriequeueNodeConfig(node_index, jason_map);
//	return rtq_config;
//}
//
//void server::SCoRe::Stop() {}
//
//server::SCoRe::SCoRe(int argc, char **argv) {
//	std::string conf_name;
//	int node_index = 0;
//	if (argc < 2) conf_name = "../configs/config_1.json";
//	else conf_name = argv[1];
//	if (argc < 3) node_index = 0;
//	else node_index = atoi(argv[2]);
//	int root = 0;
//	// make all configs here instead and pass them to SCoRe init config
//	auto json_conf_maps = conf::config_mapper(conf_name);
//	std::unordered_map<int, ReverseTrieQueueNodeConfig> config_map;
//	for (auto i : json_conf_maps){
//		ReverseTrieQueueNodeConfig conf = LoadConfiguration(conf_name, i.first);
//		config_map.insert({{i.first, conf}});
//	}
//
//	score = std::make_shared<ReverseTrieQueue>(config_map, root);
//}
