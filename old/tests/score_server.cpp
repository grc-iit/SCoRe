//
// Created by neeraj on 4/19/20.
//

#include "../include/SCoReServer.h"


int te=0;
double cap(){
	return te*322.14;
}

int main(){

	d_dict val;
	unsigned int microseconds = 1000;
	auto url = "tcp://127.0.0.1";
	auto topic_1 = "q_1";
	auto topic_2 = "q_2";


	QueueKey key_1 = QueueKey(1, 3, QueueType(QueueValue::NODE_CAPACITY, microseconds), Mode::SERVER);
	QueueConfig config_1(key_1, url, topic_1, Mode::SERVER, cap);
	ReverseTrieQueueNodeKey rtqn_key_1(1, 0, ReverseTriequeueNodeType::LEAF);
	ReverseTrieQueueNodeConfig rt_config_1 (rtqn_key_1, {config_1},  {}, 1);
//	ReverseTrieQueueNode cap_node(rt_config_1);

	QueueKey key_2 = QueueKey(2, 2, QueueType(QueueValue::TIER_CAPACITY, microseconds+20), Mode::SERVER);
	QueueConfig config_2(key_2, url, topic_2, Mode::SERVER, NULL);
	ReverseTrieQueueNodeKey rtqn_key_2(0, 0, ReverseTriequeueNodeType::NODE);
	ReverseTrieQueueNodeConfig rt_config_2 (rtqn_key_2, {config_2},  {1}, 0);
//	ReverseTrieQueueNode tier_cap_node(rt_config_2);

	std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list {{0, rt_config_1}, {1, rt_config_2}};

	SCoReServer server(node_list);
	server.server->run();
	return 0;
}