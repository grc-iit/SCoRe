//
// Created by jaime on 10/26/20.
//

#include "../../../include/score/factVertex/FactCollector.h"

std::pair<double, double> FactCollector::MemUsed() {
    FILE *pipe = popen("free -m | awk '$1==\"Mem:\" {print $4/$2*100}'", "r");
    char buf[128];
    fgets(buf, sizeof(buf), pipe);
    pclose(pipe);
    return std::make_pair(time->endTime(), atof(buf));
}

void FactCollector::Run(std::future<void> futureObj, common::daemon<FactCollector> *obj) {
    daemon = obj;
    RunInternal(std::move(futureObj));
}

void FactCollector::RunInternal(std::future<void> futureObj) {
    while(futureObj.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout) {
        // Need to push onto queue?
        MemUsed();
    }
}
