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

QueueType QueueType::operator=(QueueType *obj) {
	value = obj->value;
	interval = obj->interval;
	return *this;
}

bool QueueType::operator==(QueueType obj) {
	if (obj.value == value)
		if (obj.interval == interval)
			return true;
	return false;
}
