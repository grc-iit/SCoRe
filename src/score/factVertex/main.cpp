//
// Created by jaime on 10/26/20.
//

#include <mpi.h>
#include <common/daemon.h>
#include <score/factVertex/FactCollector.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    BASKET_CONF->BACKED_FILE_DIR="/home/kbateman/test_score"; // FIXME: Make configurable
    std::string log = "./score_fact_collector.log";
    auto daemon = basket::Singleton<common::Daemon<FactCollector>::GetInstance(log);
    daemon->Run();
    MPI_Finalize();
    return 0;
}