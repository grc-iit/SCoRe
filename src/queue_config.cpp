//
// Created by neeraj on 4/27/20.
//

#include "queue_config.h"


QueueConfig::QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode, double_function hook) {
	key_ = key;
	url_ = url;
	topic_ = topic;
	mode_ = mode;
	hook_ = hook;
}

QueueConfig::QueueConfig(QueueKey key, std::string url, std::string topic, Mode mode) {
	key_ = key;
	url_ = url;
	topic_ = topic;
	mode_ = mode;
	hook_ = nullptr;
}

QueueConfig::QueueConfig(const QueueConfig &obj) {
	key_ = obj.key_;
	url_ = obj.url_;
	topic_ = obj.topic_;
	mode_ = obj.mode_;
	hook_ = obj.hook_;
}