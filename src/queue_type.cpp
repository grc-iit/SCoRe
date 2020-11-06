//
// Created by neeraj on 4/26/20.
//

#include "queue_type.h"

bool QueueType::operator==(QueueType obj) const{
	if (obj.value_ == value_ && obj.base_interval_ == base_interval_ && obj.increase_factor_ == increase_factor_
	&& obj.pythio_interval_ == pythio_interval_) {
		return true;
	}
	return false;
}
