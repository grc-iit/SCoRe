//
// Created by neeraj on 4/18/20.
//

#ifndef SCORE_QUEUE_KEY_H
#define SCORE_QUEUE_KEY_H

#include <cstdint>
#include "common/queue/queue_type.h"
#include "common/enumerations.h"


class QueueKey {
public:
	uint16_t node_index_;
	uint16_t tier_index_;
	const QueueType type_;
	Mode mode_;

	QueueKey();
	QueueKey(uint16_t node_index, uint16_t tier_index, QueueType type, Mode mode);
	QueueKey(QueueKey const &obj);
	QueueKey &operator=(const QueueKey obj);
	bool operator==(const QueueKey &obj);
	bool operator==(const QueueKey obj) const;
};


#endif //SCORE_QUEUE_KEY_H
