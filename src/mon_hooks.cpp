//
// Created by neeraj on 4/27/20.
//

#include <common/debug.h>
#include <memory>
#include "mon_hooks.h"
#include <constants.h>
#include <cmath>

double mon::load_hook() {
    AUTO_TRACER("Hooks:load_hook");
    return 100; //100 * (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - SCORE_START_TIME);
	//return 50.0;
}

double mon::cap_hook() {
    AUTO_TRACER("Hook::GetCurrentCapacity");
    /*auto was_changed = is_changed.Get((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)));
    if (!was_changed.first || was_changed.second == 0) {*/
    /* run system command du -s to calculate size of director. */
    // /mnt/ssd/jcernudagarcia
    // /mnt/hdd/jcernudagarcia
    std::string cmd = "df | awk '{if ($6 ~ \"/\") print $4}'";
    FILE *fp;
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    auto val = std::stoll(result);
    /* is_changed.Put((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)), val);*/
    return val;
    /* } else return was_changed.second;*/
}

double mon::avail_hook() {
    AUTO_TRACER("Hooks:avail_hook");
    return 1.00;
}

double mon::memory_hook() {
    AUTO_TRACER("Hook::GetCurrentCapacity");
    std::string cmd = "du -sb /mnt/nvme/jcernudagarcia/tempfs/ | awk '{print $1}'";
    FILE *fp;
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    auto val = std::stoll(result);
    /* is_changed.Put((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)), val);*/
    return val;
    /* } else return was_changed.second;*/
}

double mon::nvme_hook() {
    AUTO_TRACER("Hook::GetCurrentCapacity");
    std::string cmd = "du -sb /mnt/nvme/jcernudagarcia/apollo_nvme/ | awk '{print $1}'";
    FILE *fp;
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    auto val = std::stoll(result);
    /* is_changed.Put((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)), val);*/
    return val;
    /* } else return was_changed.second;*/
}

double mon::ssd_hook() {
    AUTO_TRACER("Hook::GetCurrentCapacity");
    std::string cmd = "du -sb /mnt/ssd/jcernudagarcia/pvfs2-storage/data/ | awk '{print $1}'";
    FILE *fp;
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    auto val = std::stoll(result);
    /* is_changed.Put((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)), val);*/
    return val;
    /* } else return was_changed.second;*/
}

void mon::InitSimHook(std::string tracefile, std::string feature) {
    AUTO_TRACER("Hooks::simulation_hook_init");
    if (tracefile == "") {
        return;
    }
    std::ifstream tracestream(tracefile.data(), std::ifstream::in);
    const int bufsize = 1000;
    char buf[bufsize];
    tracestream.getline(buf, bufsize);
    // first line is an indexing line, it will start with # and tell us what features the dataset contains, one of which must be "timestamp"
    if (buf[0] != '#') {
        std::cerr << "Improper trace file, does not conform to specification";
        return;
    }
    std::vector<std::string> features;
    char featurebuf[bufsize];
    std::istringstream featurestream(buf+2);
    int tsindex;
    int ftindex;
    while (featurestream.getline(featurebuf, bufsize, ';')) {
        features.emplace_back(std::string(featurebuf));
        if (std::string(featurebuf) == "timestamp") {
            tsindex = features.size() - 1;
        }
        if (std::string(featurebuf) == feature) {
            ftindex = features.size() - 1;
        }
    }
    long start_timestamp = -1;
    while (tracestream.getline(buf, bufsize)) {
        featurestream.str(buf);
        featurestream.clear();
        std::string timestamp;
        std::string featureval;
        for (int i = 0; featurestream.getline(featurebuf, bufsize, ';'); i++) {
            if (i == tsindex) {
                timestamp = std::string(featurebuf);
            }
            if (i == ftindex) {
                featureval = std::string(featurebuf);
            }
        }
        std::tm ts{};
        std::istringstream timestream(timestamp);
        timestream >> std::get_time(&ts, ":%Y-%m-%d %H::%M::%S UTC:");
        std::time_t unix_timestamp = std::mktime(&ts);
        if (start_timestamp == -1) {
            start_timestamp = (long)unix_timestamp;
        }
        trace_sim_end_time = (long)unix_timestamp - start_timestamp;
        trace_simulation_map[(long)unix_timestamp - start_timestamp] = std::strtod(featureval.data(), NULL);
    }
    //tracestream.close();
}

double mon::CallSimHook() {
    AUTO_TRACER("Hooks::simulation_hook");
    long index = ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - SCORE_START_TIME) % (trace_sim_end_time + 1));
    for (auto i = trace_simulation_map.begin(); i != trace_simulation_map.end(); i++) {
        if (i->first >= index) {
            return i->second;
        }
    }
    return -1;
}