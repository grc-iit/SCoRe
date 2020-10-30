//
// Created by jaime on 10/29/20.
//
#include "include/SCoRe.h"
#include <unistd.h>

int main(int argc, char*argv[]) {
    // if root load score
    // add necessary argc srv for score . move score defs into one class
    //
    std::string conf_name;
    int node_index = 0;
    if (argc < 2) conf_name = "configs/config_2_2.json";
    else conf_name = argv[1];

    client::SCoRe insight(conf_name, "TIER_CAPACITY");
    client::SCoRe ssd_capacity(conf_name, "CAPACITY_MEMORY", 1);
    client::SCoRe mem_capacity(conf_name, "CAPACITY_SSD", 1);


    std::pair<std::string, std::string> result;

    for(int i=0; i < 5; i++) {
        result = insight.get_latest();
        std::cout << "Current Insight Value:   (" << result.first << "," << result.second << ")" << std::endl;
        result = ssd_capacity.get_latest();
        std::cout << "Current Memory Capacity: (" << result.first << "," << result.second << ")" << std::endl;
        result = mem_capacity.get_latest();
        std::cout << "Current SSD Capacity:    (" << result.first << "," << result.second << ")" << std::endl;
        sleep(35);
    }

    return 0;
}
