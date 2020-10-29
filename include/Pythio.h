//
// Created by jaime on 10/28/20.
//

#ifndef SCORE_PYTHIO_H
#define SCORE_PYTHIO_H


#include <string>
#include "enum.h"
#include "redis.h"

class Pythio {
    Model model_;
    std::string weights_;

public:
    Pythio(Model model, std::string weights):model_(model), weights_(weights){
        //Do Some initialization, aka load the module
    };
    Pythio(const Pythio &obj)=default;

    d_dict Predict(std::vector<d_dict> data_points);
    d_dict Feedback(std::vector<d_dict> data_points);

};


#endif //SCORE_PYTHIO_H
