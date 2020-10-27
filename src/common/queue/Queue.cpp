#include "score/common/queue/Queue.h"

// publishes a value to the queue
std::string Queue::publish(std::unordered_map<std::string, std::string> value) {
	return redis_->publish(value);
}

item_stream Queue::subscribe() {
	// function to get all from the latest sub id
	item_stream result = redis_->subscribe_all(lastSubscribeID_);
    lastSubscribeID_ = result.back().first;
	return result;
}