//
// Created by neeraj on 4/18/20.
//
#pragma once

#ifndef SCORE_QUEUE_TYPE_H
#define SCORE_QUEUE_TYPE_H

#include <cstdint>
#include <string>
#include "enum.h"


class QueueType  {
public:

	QueueValue value_;
	uint64_t base_interval_;
	float increase_factor_;
	uint64_t pythio_interval_;
	std::string tracefile_;
	std::string tracevar_;


	QueueType()=default;
	QueueType(QueueValue v, int base_interval, float increase_factor, int pythio_interval, std::string tracefile, std::string tracevar):value_(v),
			base_interval_(base_interval), increase_factor_(increase_factor), pythio_interval_(pythio_interval),
			tracefile_(tracefile), tracevar_(tracevar){}
	QueueType(const QueueType  &type)=default;
    QueueType &operator=(const QueueType &other)=default;
	bool operator==(QueueType obj) const;
};


#endif //SCORE_QUEUE_TYPE_H
