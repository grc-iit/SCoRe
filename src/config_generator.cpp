//
// Created by neeraj on 4/30/20.
//
#include "config_generator.h"


std::unordered_map<int, conf::json> conf::config_mapper(std::string config_filename) {

	std::ifstream file_(config_filename);
	std::unordered_map<int, conf::json> jason_map;
	conf::json jayson;
	file_ >> jayson;
	for (conf::json i : jayson) {
		jason_map.insert({{i["key"]["node_index"], i}});
	}

	return jason_map;
}

auto conf::map_all_to_ReverseTriequeueNodeConfig(std::unordered_map<int, conf::json> config_maps) {
	std::unordered_map<int, ReverseTrieQueueNodeConfig> rqtn_configs;
	for (auto i: config_maps) {
		rqtn_configs.insert({{i.first, map_to_ReverseTriequeueNodeConfig(i.first, config_maps)}});
	}
	return rqtn_configs;
}

ReverseTrieQueueNodeConfig
conf::map_to_ReverseTriequeueNodeConfig(int me, std::unordered_map<int, conf::json> index_to_json_map) {

	conf::json rtq_node = index_to_json_map[me];
	std::string hostname = strip(rtq_node["hostname"]);

	std::vector<QueueConfig> queue_configs;
	auto key = json_to_ReverseTriequeueNodeKey(rtq_node["key"]);

	std::vector<json> queue_confs = rtq_node["queues"];
	for (json queue_conf: queue_confs) {
		queue_configs.push_back(std::move(json_to_QueueConfig(queue_conf)));
	}

	std::vector<std::shared_ptr<ReverseTrieQueueNodeConfig>> children_configs;
	std::vector<int> rtq_children_vec = rtq_node["children"];
    std::vector<int> children_topics = rtq_node["children_queues"];

    std::vector<QueueConfig> child_queue_configs;
    for (size_t index = 0; index < rtq_children_vec.size(); ++index){
        int children_id = rtq_children_vec[index];
        int queue_id = children_topics[index];

        conf::json rtq_node_children = index_to_json_map[children_id];
        std::vector<json> children_queue_confs = rtq_node_children["queues"];
        json children_queue_conf = children_queue_confs[queue_id];
        QueueConfig queueConfig = json_to_QueueConfig(children_queue_conf);
        child_queue_configs.push_back(std::move(queueConfig));
    }
	auto t = ReverseTrieQueueNodeConfig(key, hostname, queue_configs, child_queue_configs, key.node_index_);
	return t;
}

ReverseTrieQueueNodeKey conf::json_to_ReverseTriequeueNodeKey(conf::json rtq_key_json) {
	std::string rtqt = strip(rtq_key_json["type"]);
	ReverseTriequeueNodeType rtq_type = rtqt.compare("ROOT") ? ReverseTriequeueNodeType::ROOT :
	                                    rtqt.compare("NODE") ? ReverseTriequeueNodeType::NODE :
	                                    rtqt.compare("LEAF") ? ReverseTriequeueNodeType::LEAF :
	                                    ReverseTriequeueNodeType::RTQ_UNDEFINED;
	return ReverseTrieQueueNodeKey(rtq_key_json["node_index"], rtq_key_json["level"], rtq_type);
}

QueueConfig conf::json_to_QueueConfig(conf::json queue_config) {
	std::string hook_ = strip(queue_config["hook"]);

	double_function hook = hook_.compare("CAPACITY") ? mon::cap_hook :
	                       hook_.compare("LOAD") ? mon::load_hook :
	                       hook_.compare("AVAILABILITY") ? mon::avail_hook :
	                       NULL;

	auto topic = strip(queue_config["topic"]);
	auto url = strip(queue_config["url"]);

	std::string queue_port = strip(queue_config["queue_port"]);

    json pyhtio_key_json = queue_config["pythio"];
    Model model = pyhtio_key_json["model"];
    std::string weights = pyhtio_key_json["weights"];

	json queue_key_json = queue_config["key"];
	QueueKey queue_key = json_to_QueueKey(queue_key_json);

	std::string mode_ = strip(queue_key_json["mode"]);
	Mode mode = mode_.compare("SERVER") ? Mode::SERVER : Mode::CLIENT;

	return QueueConfig(queue_key, url, topic, mode, hook, model, weights, std::stoi(queue_port));

}

QueueKey conf::json_to_QueueKey(conf::json queue_key_json) {
	int node_index = queue_key_json["node_index"];
	auto tier_index = queue_key_json["tier_index"];
	json queue_type_json = queue_key_json["type"];
	QueueType queue_type = json_to_QueueType(queue_type_json);
	std::string mode_ = queue_key_json["mode"];
	Mode mode = mode_.compare("SERVER") ? Mode::SERVER : Mode::CLIENT;

	return QueueKey(node_index, tier_index, queue_type, mode);
}

QueueType conf::json_to_QueueType(conf::json queue_type_json) {
	int interval = queue_type_json["interval"];
	auto value = strip(queue_type_json["value"]);
	// can be expanded QueueValue for more shyte
	QueueValue queue_value = value.compare("NODE_CAPACITY") ? QueueValue::NODE_CAPACITY :
	                         value.compare("NODE_LOAD") ? QueueValue::NODE_LOAD :
	                         value.compare("NODE_AVAILABILITY") ? QueueValue::NODE_AVAILABILITY :
	                         value.compare("TIER_CAPACITY") ? QueueValue::TIER_CAPACITY :
	                         value.compare("TIER_LOAD") ? QueueValue::TIER_LOAD :
	                         value.compare("TIER_AVAILABILITY") ? QueueValue::TIER_AVAILABILITY :
	                         value.compare("CLUSTER_CAPACITY") ? QueueValue::CLUSTER_CAPACITY :
	                         value.compare("CLUSTER_LOAD") ? QueueValue::CLUSTER_LOAD :
	                         value.compare("CLUSTER_AVAILABILITY") ? QueueValue::CLUSTER_AVAILABILITY :
	                         QueueValue::Q_UNDEFINED;

	auto queue_type_ = QueueType(queue_value, interval);
	return queue_type_;
}

std::string conf::strip(std::string s) {
	s.erase(
			remove(s.begin(), s.end(), '\"'),
			s.end()
	);
	return s;
}
