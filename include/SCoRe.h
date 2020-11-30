//
// Created by neeraj on 4/18/20.
//

#ifndef SCORE_SCORE_H
#define SCORE_SCORE_H

#include <rpc/server.h>
#include <rpc/client.h>
#include "redis.h"

namespace client {
	class SCoRe {
		std::shared_ptr<redis_client> client_;

	public:
        SCoRe(std::string config_path, std::string topic, int node);
        SCoRe(std::string config_path, std::string topic);
        std::pair<std::string, std::string> get_latest();
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
