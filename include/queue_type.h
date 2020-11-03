//
// Created by neeraj on 4/18/20.
//
#pragma once

#ifndef SCORE_QUEUE_TYPE_H
#define SCORE_QUEUE_TYPE_H

#include <cstdint>
#include "enum.h"


class QueueType  {
public:

	QueueValue value;
	uint64_t interval;

	QueueType(const QueueType  &type);
	QueueType(QueueValue v, float intrv);
	QueueType();
    QueueType &operator=(const QueueType &other);
	bool operator==(QueueType obj) const;
};


#endif //SCORE_QUEUE_TYPE_H
