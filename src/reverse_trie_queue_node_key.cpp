//
// Created by neeraj on 4/26/20.
//

#include "reverse_trie_queue_node_key.h"

ReverseTrieQueueNodeKey::ReverseTrieQueueNodeKey(uint16_t node_index, uint16_t level, ReverseTriequeueNodeType type) {
	node_index_ = node_index;
	level_ = level;
	type_ = type;

}

ReverseTrieQueueNodeKey::ReverseTrieQueueNodeKey(const ReverseTrieQueueNodeKey &key) {
	node_index_ = key.node_index_;
	level_ = key.level_;
	type_ = key.type_;
}

ReverseTrieQueueNodeKey &ReverseTrieQueueNodeKey::operator=(ReverseTrieQueueNodeKey *key) {
	node_index_ = key->node_index_;
	level_ = key->level_;
	type_ = key->type_;
	return *this;
}

ReverseTrieQueueNodeKey::ReverseTrieQueueNodeKey() {
	node_index_ = 0;
	this->type_ = ReverseTriequeueNodeType::RTQ_UNDEFINED;
	this->level_ = 0;
}
