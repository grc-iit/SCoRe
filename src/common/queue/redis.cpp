#include "score/common/queue/redis.h"

// gets everything from ls_id (list id)
item_stream redis_client::subscribe_all(std::string ls_id) {
    item_stream result;
    this->redis_->xrange(topic_, ls_id, "+", std::back_inserter(result));
    return result;
}

// publishs val to the queue, the topic is fixed per queue
std::string redis_client::publish(d_dict val) {
    return this->redis_->xadd(topic_, "*", val.begin(), val.end());
}
