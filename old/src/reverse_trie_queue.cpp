//
// Created by neeraj on 4/26/20.
//

#include "reverse_trie_queue.h"

ReverseTrieQueue::ReverseTrieQueue(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list, int root_index) {
	// setup the RTQ 
	// creates all the other nodes
	// client nodes do not run with the async loop
	// so you need to skip those and do that finally at the end
	std::unordered_map<int, std::shared_ptr<ReverseTrieQueueNode>> nodes_made;
	ReverseTrieQueueNodeConfig root_conf = node_list[root_index];

	for (auto i: node_list) {
		// add cehck for root to init it to server.
		auto t_node = new ReverseTrieQueueNode(node_list[i.first]);
		auto mode = i.first == root_index ? Mode::SERVER : Mode::CLIENT;
		if (mode == Mode::SERVER){
			break;  // server starts a loop, do it last
		}
		ReverseTrieQueueNode(i.second, mode);
		nodes_made.insert({{i.first, std::shared_ptr<ReverseTrieQueueNode>(t_node)}});
	}
//	auto root_ = ReverseTrieQueueNode(root_conf, Mode::SERVER);
	auto t_node = new ReverseTrieQueueNode(root_conf, Mode::SERVER);

	root = std::make_shared<ReverseTrieQueueNode>(root_conf, Mode::SERVER);

}
