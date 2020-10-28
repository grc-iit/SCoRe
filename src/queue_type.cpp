//
// Created by neeraj on 4/26/20.
//

#include "queue_type.h"

QueueType::QueueType(const QueueType &type) {
	this->value = type.value;
	this->interval = type.interval;

}

QueueType::QueueType(QueueValue v, float intrv) {
	value = v;
	interval = intrv;
}

QueueType::QueueType() {
	value = QueueValue::Q_UNDEFINED;
	interval = 10000;
}

QueueType &QueueType::operator=(const QueueType &other){
	value = other.value;
	interval = other.interval;
	return *this;
}

bool QueueType::operator==(QueueType obj) const{
	if (obj.value == value)
		if (obj.interval == interval)
			return true;
	return false;
}
