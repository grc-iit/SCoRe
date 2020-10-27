//
// Created by jaime on 10/26/20.
//

#include <mpi.h>
#include <basket.h>
#include <common/daemon.h>
#include <common/arguments.h>
#include <score/insightVertex/InsightVertex.h>

class InsightVertexArgs : public common::args::ArgMap {
private:
    void VerifyArgs(void) {}

public:
    void Usage(void) {
        std::cout << "Usage: ./job_manager -[param-id] [value] ... " << std::endl;
        std::cout << "-conf [string]: The config file for sentinel. Default is no config." << std::endl;
    }

    InsightVertexArgs(int argc, char **argv) {
        AddOpt("-conf", common::args::ArgType::kString, "");
        ArgIter(argc, argv);
        VerifyArgs();
    }
};

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    MPI_Barrier(MPI_COMM_WORLD);
    InsightVertexArgs args(argc, argv);
//    COMMON_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
//    SENTINEL_CONF->CONFIGURATION_FILE = args.GetStringOpt("-conf");
    CharStruct log = "./single_node_insightVertex.log";
//    COMMON_CONF->LoadConfiguration();
    auto daemon = basket::Singleton<common::Daemon<score::InsightVertex>>::GetInstance(log);
    daemon->Run();
    return 0;
}