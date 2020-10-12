
#include "include/SCoRe.h"
#include "include/reverse_trie_queue.h"
#include "include/daemonize.h"
#include "include/cxxopts/cxxopts.hpp"

unsigned long long int size_parser(std::string size_string){
	std::size_t MB = size_string.find("MB");
	std::size_t KB = size_string.find("KB");
	std::size_t B = size_string.find("B");
	std::size_t b = size_string.find("b");

	return MB != std::string::npos ? 1024 * 1024 * std::stoull(size_string.substr(0, MB - 1)) :
	       KB != std::string::npos ? 1024 * std::stoull(size_string.substr(0, KB - 1)) :
	       B != std::string::npos ? 8 * std::stoull(size_string.substr(0, B - 1)) :
	       std::stoull(size_string.substr(0, b - 1));
}


int main(int argc, char*argv[]) {
	// if root load score
	// add necessary argc srv for score . move score defs into one class
	//

	cxxopts::Options options("Bench", "A Benchmark tool for SCoRe");

	options.add_options()
			("b,bar", "Param bar", cxxopts::value<std::string>())
			("m, message", "Number of messages", cxxopts::value<int>()->default_value("10"))
			("s,size", "Message size", cxxopts::value<std::string>()->default_value("4KB"))
			("h,help", "Print this menu")
			;

	auto result = options.parse(argc, argv);
// print help menu when nothing is sent to it.

	std::string conf_name;
	int node_index = 0;
	if (argc < 2) conf_name = "configs/config_1.json";
	else conf_name = argv[1];
	if (argc < 3) node_index = 0;
	else node_index = atoi(argv[2]);

	std::cout<<"CONF: "<<conf_name<<", NODE INDEX: "<<node_index<<std::endl;
	if (node_index == 0 ) auto daemon_i=Singleton<Daemon<server::SCoRe>>::GetInstance(argc,argv);
	else auto daemon_i=Singleton<Daemon<ReverseTrieQueueNode>>::GetInstance(argc,argv);

	std::cout << "Hello there";

	return 0;
}
