//
// Created by jaime on 10/28/20.
//

#include "Pythio.h"

d_dict Pythio::Predict(std::vector<d_dict> data_points) {
    return data_points.back();
}

d_dict Pythio::Feedback(std::vector<d_dict> data_points) {
    return d_dict();
}
