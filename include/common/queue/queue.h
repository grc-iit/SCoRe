#pragma once

#ifndef SCORE_QUEUE_H
#define SCORE_QUEUE_H

#include "queue_config.h"
#include "../../../old/include/enum.h"
#include "queue_key.h"
#include "redis.h"
#include <ctime>
#include <string>
#include <unordered_map>

#ifdef BENCH_TIMER
#include "timer.h"
#endif


template<>
struct std::hash<QueueValue> {
public:
	size_t operator()(const QueueValue &t) const { return size_t(t); }
};

template<>
class std::hash<QueueType> {
public:
	int operator()(const QueueType &q) const {
		size_t hash_val = hash<QueueValue>()(q.value);
		hash_val ^= hash<float>()(q.interval);
		return hash_val;
	}
};

template<>
class std::hash<QueueKey> {
public:
	int operator()(const QueueKey &q) const {

		size_t hash_val = hash<QueueType>()(q.type_);
		hash_val ^= hash<uint16_t>()(q.tier_index_);
		hash_val ^= hash<uint16_t>()(q.node_index_);
		hash_val ^= hash<Mode>()(q.mode_);
		return hash_val;
	}

};

class queue;
using QM_type = std::unordered_map<QueueKey, std::shared_ptr<queue>>;


class queue {
public:
	std::shared_ptr<redis_client> redis;
	QueueKey key;
	std::string lat_pub_id;
	std::string lat_sub_id;
	d_dict lat_published;
	double_function mon_hook;

	queue(queue const &pQueue);
	queue(QueueConfig config);
	queue(QueueKey kei, std::string url, std::string topic);
	std::string publish(d_dict value);
	item_stream subscribe();
	d_dict get_latest();
	bool is_synced(std::vector<QM_type> children);
	virtual std::string populate();
	virtual std::string populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps);
};


#endif

