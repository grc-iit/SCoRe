#pragma once


#include <rpc/server.h>
#include "reverse_trie_queue.h"
#include <unordered_map>

using SCoRe_return = std::unordered_map<std::string, d_dict>;

class SCoReServer {
	ReverseTrieQueue *score;

public:

	auto get_latest();

	SCoReServer(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list);

	rpc::server *server;
};
