#pragma once
#ifndef SCORE_S_QUEUES_H
#define SCORE_S_QUEUES_H


#include"queue.h"
#include"enum.h"


class capacity_queue : public queue {
public:
	capacity_queue(QueueConfig config) : queue(config) {}
	std::string populate(
			std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue> >> child_queue_maps);
};

class load_queue : public queue {
public:
	load_queue(QueueConfig config) : queue(config) {}

	std::string populate(
			std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue> >> child_queue_maps);
};

class availability_queue : public queue {
public:
	availability_queue(QueueConfig config) : queue(config) {}
	std::string populate(
			std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue> >> child_queue_maps);
};

class mon_queue : public queue {
public:
	mon_queue(QueueConfig config) : queue(config) {}};

#endif