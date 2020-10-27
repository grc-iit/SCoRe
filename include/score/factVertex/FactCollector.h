//
// Created by jaime on 10/26/20.
//

#ifndef SCORE_FACTCOLLECTOR_H
#define SCORE_FACTCOLLECTOR_H

#include <common/debug.h>
#include <cstdlib>
#include <cstdio>

class FactCollector {
private:
    common::debug::Timer *time;
public:
    FactCollector() {
        time = new common::debug::Timer();
        time->startTime();
    }
    ~FactCollector() {
        delete time;
    }
    std::pair<double, double> MemUsed();
};


#endif //SCORE_FACTCOLLECTOR_H
