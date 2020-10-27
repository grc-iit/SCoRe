#pragma once

#ifndef SCORE_REDIS_H
#define SCORE_REDIS_H

#include <sw/redis++/redis++.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using d_dict = std::unordered_map<std::string, std::string>;
using item = std::pair<std::string, d_dict>;
using item_stream = std::vector<item>;

class redis_client {

private:
	std::string url;
	d_dict latest_entry;

public:
	std::shared_ptr<sw::redis::Redis> redis;
	std::string latest_pub_id;
	std::string latest_sub_id;
	std::string topic;
	bool sub_sync;
	item last_item;

	redis_client(std::string url, std::string topic);
	redis_client &operator=(const redis_client obj);
	[[maybe_unused]] item_stream subscribe_all();
	item_stream subscribe_all(std::string ls_id);
	std::string publish(d_dict val);
	std::optional<item> subscribe_next();
};

#endif