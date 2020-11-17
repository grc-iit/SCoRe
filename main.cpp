
#include "include/SCoRe.h"
#include "include/reverse_trie_queue.h"
#include "include/daemonize.h"

int main(int argc, char*argv[]) {
	// if root load score
	// add necessary argc srv for score . move score defs into one class
	//
	std::string conf_name;
	std::string pid_file;
	int node_index = 0;
	if(argc == 4){
        conf_name = argv[1];
        node_index = atoi(argv[2]);
        pid_file = argv[3];
	}
	else{
	    exit(1);
	}

	std::cout<<"CONF: "<<conf_name<<", NODE INDEX: "<<node_index<<std::endl;
    auto daemon_i=Singleton<Daemon<ReverseTrieQueueNode>>::GetInstance(argc,argv, true, pid_file);
//    auto daemon_i=Singleton<Daemon<ReverseTrieQueueNode>>::GetInstance(argc,argv);

    std::cout << "Hello there";

    return 0;
}
