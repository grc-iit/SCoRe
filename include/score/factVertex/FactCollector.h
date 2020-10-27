//
// Created by jaime on 10/26/20.
//

#ifndef SCORE_FACTCOLLECTOR_H
#define SCORE_FACTCOLLECTOR_H

#include <score/common/queue/Queue.h>
#include <common/debug.h>
#include <common/daemon.h>
#include <cstdlib>
#include <cstdio>

class FactCollector {
private:
    common::debug::Timer *time;
    common::Daemon<FactCollector> * daemon;
    Queue *statQueue;
public:
    FactCollector() {
        time = new common::debug::Timer();
        time->startTime();
        statQueue = new Queue("localhost:6379", "test"); // FIXME make configurable
    }
    ~FactCollector() {
        delete time;
        delete statQueue;
    }
    std::pair<double, double> MemUsed();
    void Run(std::future<void> futureObj, common::daemon<FactCollector> *obj);
    void RunInternal(std::future<void> futureObj);
};


#endif //SCORE_FACTCOLLECTOR_H
