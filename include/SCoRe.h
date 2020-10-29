//
// Created by neeraj on 4/18/20.
//

#ifndef SCORE_SCORE_H
#define SCORE_SCORE_H

#include <rpc/server.h>
#include <rpc/client.h>
#include "redis.h"
#include "reverse_trie_queue.h"

namespace client {
	class SCoRe {
		rpc::client *client_;
		std::string url_;
		uint16_t port_;

	public:
		SCoRe();

		SCoRe(std::string url, uint16_t port);

		void get_latest();
	};
}

//typedef std::unordered_map<std::string, d_dict> SCoRe_return;
//namespace server{
//
//	class SCoRe {
//
//	protected:
//		std::shared_ptr<ReverseTrieQueue> score;
//
//	public:
//
//		auto get_latest();
//		SCoRe(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list);
//		ReverseTrieQueueNodeConfig LoadConfiguration(std::string basicString, int index);
//		void Stop();
//		SCoRe(int argc, char* argv[]);
//		rpc::server *server;
//	};
//}

#endif //SCORE_SCORE_H
