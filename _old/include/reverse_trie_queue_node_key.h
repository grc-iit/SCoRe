//
// Created by neeraj on 4/18/20.
//
#pragma once
#ifndef SCORE_REVERSE_TRIE_QUEUE_NODE_KEY_H
#define SCORE_REVERSE_TRIE_QUEUE_NODE_KEY_H


#include <cstdint>
#include "enum.h"

class ReverseTrieQueueNodeKey {

public:
	uint16_t node_index_;
	uint16_t level_;
	ReverseTriequeueNodeType type_;

	ReverseTrieQueueNodeKey(uint16_t node_index, uint16_t level, ReverseTriequeueNodeType type);
	ReverseTrieQueueNodeKey(const ReverseTrieQueueNodeKey &key);
	ReverseTrieQueueNodeKey &operator=(ReverseTrieQueueNodeKey *key);
	ReverseTrieQueueNodeKey();

};


#endif //SCORE_REVERSE_TRIE_QUEUE_NODE_KEY_H
