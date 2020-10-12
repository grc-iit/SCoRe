//
// Created by neeraj on 4/18/20.
//

/*
 * Daemonize takes inputs as main mode_ index and config
 *
 * make json parser
 * visualizer for config
 * script to spawn right exec
 *
 * ensure constants are in a config file.
 *
 *
 * */


#include <iostream>
#include "../include/reverse_trie_queue_node.h"
#include <unistd.h>

int te=0;
d_dict val;
unsigned int microseconds = 1000;
auto url = "tcp://127.0.0.1";
std::unordered_map<int,ReverseTrieQueueNodeConfig> node_map;
std::vector<std::thread> threads;
float seconds = 0.02;

void check_results();

double cap(){
	return te++*3.14;
}
int start_thread(int node_index){
	auto iter = node_map.find(node_index);
	if(iter != node_map.end()) {
		ReverseTrieQueueNode tier_cap_node(iter->second,Mode::SERVER);
	}else return -1;
	return 0;
}

int finalize(){
	check_results();

	return 0;
}

void check_results() {

}

int main(){

	node_map = std::unordered_map<int,ReverseTrieQueueNodeConfig>();
	auto topic_1 = "rq_1";
	QueueKey child_key = QueueKey(1, 0, QueueType(QueueValue::NODE_CAPACITY, microseconds), Mode::SERVER);
	QueueConfig child_queue_config(child_key, url, topic_1, Mode::SERVER, cap);
	ReverseTrieQueueNodeKey rtq_child_node_key(1, 0, ReverseTriequeueNodeType::LEAF);
	ReverseTrieQueueNodeConfig rtq_child_config (rtq_child_node_key, "localhost", {child_queue_config}, {}, 1);
	node_map.insert(std::pair<int,ReverseTrieQueueNodeConfig>(1, rtq_child_config));

	auto topic_2 = "rq_2";
	QueueKey parent_key = QueueKey(2, 0, QueueType(QueueValue::TIER_CAPACITY, microseconds), Mode::SERVER);
	QueueConfig parent_queue_config(parent_key, url, topic_2, Mode::SERVER, NULL);

	ReverseTrieQueueNodeKey rtq_parent_node_key(0, 0, ReverseTriequeueNodeType::NODE);
	auto children = std::make_shared<ReverseTrieQueueNodeConfig>(rtq_child_config);
	ReverseTrieQueueNodeConfig rtq_parent_config (rtq_parent_node_key,"localhost" ,{parent_queue_config}, {children}, 0);
	node_map.insert(std::pair<int,ReverseTrieQueueNodeConfig>(0, rtq_parent_config));


	/**
	 * Start threads for child and parent
	 */
	threads = std::vector<std::thread>();
	threads.push_back(std::thread(&start_thread,0));
	threads.push_back(std::thread(&start_thread,1));
	/**
	 * wait for completion
	 */
//	 add_signals();
	 while(true)
	 	usleep(100000);
	 return 0;
//	// sync works.. seems to be stuck after making the first node. should we have 2 different pieces of code?
//	//
//
//	std::thread node_1;
//	std::vector<QM_type> child_map = {{cap_node.queue_map}};
//
//	std::cout << "\nPopulating\n";
//
//
//	cap_node.get(child_key)->populate();
////	tier_cap_node.get(key_2)->redis->sub_sync = false;
//	auto k = tier_cap_node.get(parent_key)->populate(child_map);
//	sleep(seconds);
//
//
////	tier_cap_node.get(key_2)->populate(child_map);
//
//	cap_node.get(child_key)->populate();
//	k = tier_cap_node.get(parent_key)->populate(child_map);
//	sleep(seconds);
//
//
//	cap_node.get(child_key)->populate();
////	tier_cap_node.get(key_2)->redis->sub_sync = false;
//	k = tier_cap_node.get(parent_key)->populate(child_map);
//	sleep(seconds);
//
//	cap_node.get(child_key)->populate();
////	tier_cap_node.get(key_2)->redis->sub_sync = false;
//	k = tier_cap_node.get(parent_key)->populate(child_map);
//	sleep(seconds);

//	cap_node.get(key_1)->subscribe();
//
	std::cout << "\nsub\n";
//



/*	for (auto i : tier_cap_node.get(parent_key)->subscribe()){
		std::cout<<i.first;
		for (auto j: i.second){
			std::cout<<j.first<<' '<<j.second<<std::endl;
		}
	}*/

//	cap_node.get(key_1)->


	return 0;
}
