//
// Created by jaime on 10/26/20.
//

#ifndef SCORE_FACTCOLLECTOR_H
#define SCORE_FACTCOLLECTOR_H

#include <common/debug.h>
#include <common/daemon.h>
#include <cstdlib>
#include <cstdio>

class FactCollector {
private:
    common::debug::Timer *time;
    common::Daemon<FactCollector> * daemon;
public:
    FactCollector() {
        time = new common::debug::Timer();
        time->startTime();
    }
    ~FactCollector() {
        delete time;
    }
    std::pair<double, double> MemUsed();
    void Run(std::future<void> futureObj, common::daemon<FactCollector> *obj);
    void RunInternal(std::future<void> futureObj);
};


#endif //SCORE_FACTCOLLECTOR_H
