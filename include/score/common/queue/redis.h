#ifndef SCORE_REDIS_H
#define SCORE_REDIS_H

#include <sw/redis++/redis++.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>

using d_dict = std::unordered_map<std::string, std::string>;
using item = std::pair<std::string, d_dict>;
using item_stream = std::vector<item>;

class redis_client {

private:
    std::shared_ptr<sw::redis::Redis> redis_;
    std::string url_;
	std::string topic_;
public:
	redis_client(const std::string& url, std::string topic): redis_(std::make_shared<sw::redis::Redis>(url)),
	                                                  url_(url), topic_(std::move(topic)){}

    redis_client(const redis_client &other) = default;
    redis_client(redis_client &&other) = default;
    redis_client &operator=(const redis_client &other) = default;

	item_stream subscribe_all(std::string ls_id);
	std::string publish(d_dict val);
};

#endif