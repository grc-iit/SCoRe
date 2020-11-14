//
// Created by neeraj on 4/17/20.
//

#ifndef SCORE_MON_HOOKS_H
#define SCORE_MON_HOOKS_H

#include <map>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

namespace mon {
    static std::map<long, double> trace_simulation_map;
    static long trace_sim_end_time;
    void InitSimHook(std::string tracefile, std::string feature);
    double CallSimHook();
	double load_hook();
	double avail_hook();
	double cap_hook();
}
#endif //SCORE_MON_HOOKS_H
