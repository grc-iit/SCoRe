#ifndef SCORE_QUEUE_H
#define SCORE_QUEUE_H

#include "score/common/data_structure.h"
#include "score/common/enumerations.h"
#include "redis.h"
#include <ctime>
#include <string>
#include <unordered_map>
#include <memory>

class Queue {
    std::shared_ptr<redis_client> redis_;
    std::string lastSubscribeID_;
public:
    Queue(std::string url, std::string topic): redis_(std::make_shared<redis_client>(url, topic)),
                                                                    lastSubscribeID_(""){}
    Queue(const Queue &other) = default;
    Queue(Queue &&other) = default;
    Queue &operator=(const Queue &other) = default;

	std::string publish(d_dict value);
	item_stream subscribe();
};
#endif

