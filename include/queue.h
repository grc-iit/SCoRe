#pragma once

#ifndef SCORE_QUEUE_H
#define SCORE_QUEUE_H

#include "queue_config.h"
#include "enum.h"
#include "queue_key.h"
#include "redis.h"
#include "Pythio.h"
#include <ctime>
#include <string>
#include <unordered_map>
#include <rpc/server.h>

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
	std::shared_ptr<redis_client> redis_;
	QueueKey key_;
	std::string lat_pub_id_="1";
	std::string lat_sub_id_="1";
	std::vector<d_dict> lat_published_;
	double_function mon_hook_;
	Pythio pythio_;
    int window_size = 10;
    Mode mode_;

    std::shared_ptr<rpc::server> rpc;

	queue(queue const &pQueue)=default;
	queue(QueueConfig config):
                redis_(std::make_shared<redis_client>(config.url_, config.topic_)), key_(config.key_), lat_pub_id_("1"),
                lat_sub_id_("1"), mon_hook_(config.hook_), pythio_(Pythio(config.model_, config.weights_)), mode_(config.mode_){}
	std::string publish(d_dict value);
	item_stream subscribe();
	d_dict get_latest();
	bool is_synced(std::vector<QM_type> children);
	virtual std::string populate();
    std::string populate_pythio();
	virtual std::string populate(std::vector<std::unordered_map<QueueKey, std::shared_ptr<queue>>> child_queue_maps);

	void queue_test(int num_repeats);
};

#endif

