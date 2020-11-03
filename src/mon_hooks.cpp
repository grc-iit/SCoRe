//
// Created by neeraj on 4/27/20.
//

#include <common/debug.h>
#include <memory>
#include "mon_hooks.h"

double mon::load_hook() {
    AUTO_TRACER("Hooks:load_hook");
	return 50.0;
}

double mon::cap_hook() {
    AUTO_TRACER("HDF5Client::GetCurrentCapacity");
    /*auto was_changed = is_changed.Get((std::to_string(HERMES_CONF->MY_SERVER)+"_"+std::to_string(layer.id_)));
    if (!was_changed.first || was_changed.second == 0) {*/
    /* run system command du -s to calculate size of director. */
    std::string cmd = "du -s -B1 /home/ | awk {'print$1'}";
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