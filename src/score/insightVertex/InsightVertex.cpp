//
// Created by jaime on 10/26/20.
//

#include "score/insightVertex/InsightVertex.h"

void score::InsightVertex::Run(std::future<void> futureObj,common::Daemon<Server> * obj) {
    daemon = obj;
    RunInternal(std::move(futureObj));
}

void score::InsightVertex::RunInternal(std::future<void> futureObj) {
    while(futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
        usleep(10000);
    }
}
