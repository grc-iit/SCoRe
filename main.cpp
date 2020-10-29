
#include "include/SCoRe.h"
#include "include/reverse_trie_queue.h"
#include "include/daemonize.h"

int main(int argc, char*argv[]) {
	// if root load score
	// add necessary argc srv for score . move score defs into one class
	//
	std::string conf_name;
	int node_index = 0;
	if (argc < 2) conf_name = "configs/config_1.json";
	else conf_name = argv[1];
	if (argc < 3) node_index = 0;
	else node_index = atoi(argv[2]);

	std::cout<<"CONF: "<<conf_name<<", NODE INDEX: "<<node_index<<std::endl;
    auto daemon_i=Singleton<Daemon<ReverseTrieQueueNode>>::GetInstance(argc,argv);

    std::cout << "Hello there";

    return 0;
}
