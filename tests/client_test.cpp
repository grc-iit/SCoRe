//
// Created by jaime on 10/29/20.
//
#include <unistd.h>
#include <SCoRe.h>
#include <timer.h>

int main(int argc, char*argv[]) {
    uint64_t changes = 0;
    uint64_t fails = 0;
    uint64_t average = 0;
    std::string old_version;

    auto redis = new sw::redis::Redis("tcp://localhost");

    Timer pop_timer;
    pop_timer.startTime();

    auto start = std::chrono::system_clock::now();
    while((std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now() - start).count() < 10)){
        item_stream data;
        redis->xrevrange("FINAL_INSIGHT", "+", "-", 1,std::back_inserter(data));
//        auto result = data.back().second.back();
        auto result = data.back();
        if(!result.first.empty() && result.first != old_version){
            changes++;
            old_version = result.first;
            uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            average += now - std::stoull(result.second.back().first);
        }
        else fails++;
    }

    pop_timer.endTimeWithPrint("Scale Test (Changes " + std::to_string(changes) + ", Fails: " + std::to_string(fails) + + ", Average: " + std::to_string(average) + "): ");

    return 0;
}
