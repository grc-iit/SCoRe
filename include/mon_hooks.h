//
// Created by neeraj on 4/17/20.
//

#ifndef SCORE_MON_HOOKS_H
#define SCORE_MON_HOOKS_H

namespace mon {
	double load_hook();
	double avail_hook();
	double cap_hook();
	double memory_hook();
	double nvme_hook();
	double ssd_hook();
}
#endif //SCORE_MON_HOOKS_H
