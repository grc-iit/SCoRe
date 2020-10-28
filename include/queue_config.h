//
// Created by neeraj on 4/27/20.
//

#ifndef SCORE_QUEUE_CONFIG_H
#define SCORE_QUEUE_CONFIG_H

#include "queue_key.h"
#include <string>

using double_function = double (*)();

class QueueConfig {
public:
	QueueKey key_;
	std::string url_;
	std::string topic_;
	Mode mode_;
	double_function hook_;
    Model model_;
    std::string weights_;

	QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode, double_function hook, Model model,
             std::string weights):key_(key),url_(url),topic_(topic),mode_(mode),hook_(hook),model_(model),
             weights_(weights){};
	QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode, Model model, std::string weights):
	        key_(key),url_(url),topic_(topic),mode_(mode),hook_(nullptr),model_(model),weights_(weights){};
	QueueConfig(const QueueConfig &obj)=default;
};
#endif //SCORE_QUEUE_CONFIG_H

