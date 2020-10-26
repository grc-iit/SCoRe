//
// Created by neeraj on 4/18/20.
//
#pragma once

#ifndef SCORE_QUEUE_TYPE_H
#define SCORE_QUEUE_TYPE_H

#include <cstdint>
#include "common/enumerations.h"


class QueueType  {
public:

	QueueValue value;
	uint16_t interval;

	QueueType(const QueueType  &type);
	QueueType(QueueValue v, float intrv);
	QueueType();
	QueueType operator=(QueueType *obj);
	bool operator==(QueueType obj);
};


#endif //SCORE_QUEUE_TYPE_H
