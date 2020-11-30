//
// Created by neeraj on 4/26/20.
//

#include "queue_key.h"

//bool QueueKey::operator==(QueueKey obj) const {
//	return obj.type_ == type_ && obj.node_index_ == node_index_ && obj.mode_ == mode_ &&
//	       obj.tier_index_ == tier_index_;
//}

bool QueueKey::operator==(const QueueKey &obj) const{
	return obj.node_index_ == node_index_ && obj.mode_ == mode_ && obj.tier_index_ == tier_index_ && type_ == obj.type_;
}

QueueKey::QueueKey(uint16_t node_index, uint16_t tier_index, QueueType type, Mode mode) {
	node_index_ = node_index;
	tier_index_ = tier_index;
	type_ = type;
	mode = mode;
}

QueueKey::QueueKey() {
	node_index_ = 0;
	tier_index_ = 0;
	type_ = QueueType();
	mode_ = Mode::SERVER;
}
