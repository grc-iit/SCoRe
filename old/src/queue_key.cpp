//
// Created by neeraj on 4/26/20.
//

#include "queue_key.h"
// why the redline?
bool QueueKey::operator==(QueueKey obj) const {
	return obj.type_ == this->type_ && obj.node_index_ == this->node_index_ && obj.mode_ == this->mode_ &&
	       obj.tier_index_ == this->tier_index_;
}

bool QueueKey::operator==(const QueueKey &obj) {
	return this->type_ == obj.type_ && obj.node_index_ == this->node_index_ && obj.mode_ == this->mode_ &&
	       obj.tier_index_ == this->tier_index_;
}

QueueKey &QueueKey::operator=(QueueKey obj) {
	type_ = obj.type_;
	mode_ = obj.mode_;
	type_ = obj.type_;
	node_index_ = obj.node_index_;
	tier_index_ = obj.tier_index_;
	return *this;
}

QueueKey::QueueKey(const QueueKey &obj) {
	this->type_ = obj.type_;
	this->mode_ = obj.mode_;
	this->type_ = obj.type_;
	this->node_index_ = obj.node_index_;
	this->tier_index_ = obj.tier_index_;
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
