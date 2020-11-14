#ifndef SCORE_ENUM_H
#define SCORE_ENUM_H


enum struct QueueValue {
	Q_UNDEFINED,
	NODE_AVAILABILITY,
	NODE_LOAD,
	NODE_CAPACITY,
	TIER_AVAILABILITY,
	TIER_LOAD,
	TIER_CAPACITY,
	CLUSTER_AVAILABILITY,
	CLUSTER_LOAD,
	CLUSTER_CAPACITY,
    SIM
};

enum struct ReverseTriequeueNodeType {
	RTQ_UNDEFINED,
	ROOT,
	NODE,
	LEAF
};

enum struct PredicateEnum {
	LAST,
	TOP_NUM
};

enum struct Mode {
	SERVER,
	CLIENT
};
enum struct Model {
    LINEAR,
    QUAD,
    OTHER
};


#endif