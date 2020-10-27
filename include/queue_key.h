//
// Created by neeraj on 4/18/20.
//

#ifndef SCORE_QUEUE_KEY_H
#define SCORE_QUEUE_KEY_H

#include <cstdint>
#include "queue_type.h"
#include "enum.h"


class QueueKey {
public:
	uint16_t node_index_;
	uint16_t tier_index_;
	QueueType type_;
	Mode mode_;

	QueueKey();
	QueueKey(uint16_t node_index, uint16_t tier_index, QueueType type, Mode mode);
    QueueKey(const QueueKey &other): node_index_(other.node_index_),tier_index_(other.tier_index_),type_(other.type_),mode_(other.mode_) {}
    QueueKey(QueueKey &other): node_index_(other.node_index_),tier_index_(other.tier_index_),type_(other.type_),mode_(other.mode_) {}
	bool operator==(const QueueKey &obj) const;
};


#endif //SCORE_QUEUE_KEY_H
