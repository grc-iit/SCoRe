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


	auto key = json_to_ReverseTriequeueNodeKey(rtq_node["key"]);
	std::vector<json> queue_confs = rtq_node["queues"];

    std::vector<QueueConfig> queue_configs;
	for (json queue_conf: queue_confs) {
	    QueueConfig queue_config = json_to_QueueConfig(queue_conf);
		queue_configs.push_back(std::move(queue_config));
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
	ReverseTriequeueNodeType rtq_type = rtqt == "ROOT" ? ReverseTriequeueNodeType::ROOT :
	                                    rtqt == "NODE" ? ReverseTriequeueNodeType::NODE :
	                                    rtqt == "LEAF" ? ReverseTriequeueNodeType::LEAF :
	                                    ReverseTriequeueNodeType::RTQ_UNDEFINED;
 	return ReverseTrieQueueNodeKey(rtq_key_json["node_index"], rtq_key_json["level"], rtq_type);
}

QueueConfig conf::json_to_QueueConfig(conf::json queue_config) {
	std::string hook_ = strip(queue_config["hook"]);

	double_function hook = hook_ == "CAPACITY" ? mon::cap_hook :
	                       hook_ == "LOAD" ? mon::load_hook :
	                       hook_ == "AVAILABILITY" ? mon::avail_hook :
	                       nullptr;

	auto topic = strip(queue_config["topic"]);
	auto url = strip(queue_config["url"]);

	int queue_port = queue_config["queue_port"];

    json pyhtio_key_json = queue_config["pythio"];
    auto json_model = strip(pyhtio_key_json["model"]);
    Model model = json_model == "MODEL_LINEAR" ? Model::LINEAR :
                  json_model == "MODEL_QUAD" ? Model::QUAD :
                  Model::OTHER;
    std::string weights = strip(pyhtio_key_json["weights"]);

	json queue_key_json = queue_config["key"];
	QueueKey queue_key = json_to_QueueKey(queue_key_json);

	std::string mode_ = strip(queue_key_json["mode"]);
	Mode mode = mode_ == "SERVER" ? Mode::SERVER : Mode::CLIENT;

	return QueueConfig(queue_key, url, topic, mode, hook, model, weights, queue_port);

}

QueueKey conf::json_to_QueueKey(conf::json queue_key_json) {
	int node_index = queue_key_json["node_index"];
	auto tier_index = queue_key_json["tier_index"];
	json queue_type_json = queue_key_json["type"];
	QueueType queue_type = json_to_QueueType(queue_type_json);
	std::string mode_ = queue_key_json["mode"];
	Mode mode = mode_ == "SERVER" ? Mode::SERVER : Mode::CLIENT;

	return QueueKey(node_index, tier_index, queue_type, mode);
}

QueueType conf::json_to_QueueType(conf::json queue_type_json) {
	int interval = queue_type_json["interval"];
	auto value = strip(queue_type_json["value"]);
	// can be expanded QueueValue for more shyte
	QueueValue queue_value = value == "NODE_CAPACITY" ? QueueValue::NODE_CAPACITY :
	                         value == "NODE_LOAD" ? QueueValue::NODE_LOAD :
	                         value == "NODE_AVAILABILITY" ? QueueValue::NODE_AVAILABILITY :
	                         value == "TIER_CAPACITY" ? QueueValue::TIER_CAPACITY :
	                         value == "TIER_LOAD" ? QueueValue::TIER_LOAD :
	                         value == "TIER_AVAILABILITY" ? QueueValue::TIER_AVAILABILITY :
	                         value == "CLUSTER_CAPACITY" ? QueueValue::CLUSTER_CAPACITY :
	                         value == "CLUSTER_LOAD" ? QueueValue::CLUSTER_LOAD :
	                         value == "CLUSTER_AVAILABILITY" ? QueueValue::CLUSTER_AVAILABILITY :
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

std::string conf::topic_to_redis(json config, std::string topic) {
    std::vector<json> queue_confs = config["queues"];
    for (json queue_conf: queue_confs) {
        if(strip(queue_conf["topic"]) == topic){
            return strip(queue_conf["url"]);
        }
    }
    return "";
}

std::string conf::topic_to_redis(std::unordered_map<int, conf::json> config, std::string topic) {
    for (auto node_config : config) {
        std::vector<json> queue_confs = node_config.second["queues"];
        for (json queue_conf: queue_confs) {
            if(strip(queue_conf["topic"]) == topic){
                return strip(queue_conf["url"]);
            }
        }
    }
    return "";
}

