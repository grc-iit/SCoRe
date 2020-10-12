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
	QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode,
	            double_function hook);

	QueueKey key_;
	std::string url_;
	std::string topic_;
	Mode mode_;
	double_function hook_;

	QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode);
	QueueConfig(const QueueConfig &obj);
};
#endif //SCORE_QUEUE_CONFIG_H
