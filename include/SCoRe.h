//
// Created by neeraj on 4/18/20.
//

#ifndef SCORE_SCORE_H
#define SCORE_SCORE_H

#include <rpc/server.h>
#include <rpc/client.h>
#include "SCoReServer.h"

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

namespace server{

	using SCoRe_return = std::unordered_map<std::string, d_dict>;

	class SCoRe {

	protected:
		std::shared_ptr<ReverseTrieQueue> score;

	public:

		auto get_latest();
		SCoRe(std::unordered_map<int, ReverseTrieQueueNodeConfig> node_list);
		ReverseTrieQueueNodeConfig LoadConfiguration(std::string basicString, int index);
		void Stop();
		SCoRe(int argc, char* argv[]);
		rpc::server *server;
	};



}

#endif //SCORE_SCORE_H
