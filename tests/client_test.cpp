//
// Created by jaime on 10/29/20.
//
#include <unistd.h>
#include <SCoRe.h>
#include <timer.h>

int main(int argc, char*argv[]) {
    uint64_t changes = 0;
    uint64_t fails = 0;
    std::string old_version;

    auto redis = new sw::redis::Redis("tcp://localhost");

    Timer pop_timer;
    pop_timer.startTime();

    auto start = std::chrono::system_clock::now();
    while((std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now() - start).count() < 2)){
        item_stream data;
        redis->xrevrange("FINAL_INSIGHT", "+", "-", 1,std::back_inserter(data));
        auto result = data.back().second.back();
        if(!result.first.empty() && result.first != old_version){
            changes++;
            old_version = result.first;
        }
        else fails++;
    }

    pop_timer.endTimeWithPrint("Scale Test (Changes " + std::to_string(changes) + ", Fails: " + std::to_string(fails) + "): ");

    return 0;
}
