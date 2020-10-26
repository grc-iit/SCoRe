//
// Created by Jie on 8/27/20.
//

#ifndef SCORE_ENUMERATIONS_H
#define SCORE_ENUMERATIONS_H

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
    CLUSTER_CAPACITY

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

#endif //SCORE_ENUMERATIONS_H

