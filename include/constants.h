//
// Created by kbateman on 11/3/20.
//

#ifndef SCORE_CONSTANTS_H

const unsigned int MIN_DELAY_TIME = 1000000;
const long SCORE_START_TIME = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

#define SCORE_CONSTANTS_H

#endif //SCORE_CONSTANTS_H
