cmake_minimum_required(VERSION 3.15)
project(SCoRe)

set(CMAKE_CXX_STANDARD 17)

#include_directories(scs_io_common/include)
include_directories(include)
set(LIBS -lpthread -lrpc  -lhiredis -lredis++ -lmpi -luv)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D BENCH_TIMER")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

set(QUEUE_COMMON src/queue.cpp include/queue.h
        src/queue_key.cpp include/queue_key.h
        src/queue_type.cpp include/queue_type.h
        src/mon_hooks.cpp include/mon_hooks.h
        include/redis.h src/redis.cpp
        include/queue_config.h src/queue_config.cpp
        include/s_queues.h src/s_queues.cpp
        include/enum.h
        include/Pythio.h src/Pythio.cpp)

set(SCORE_COMMON include/reverse_trie_queue_node_config.h src/reverse_trie_queue_node_config.cpp
        include/reverse_trie_queue.h src/reverse_trie_queue.cpp
        include/reverse_trie_queue_node.h src/reverse_trie_queue_node.cpp
        include/reverse_trie_queue_node_key.h src/reverse_trie_queue_node_key.cpp
        include/config_generator.h src/config_generator.cpp
        include/SCoRe.h src/SCoRe.cpp)

set(DEBUG_COMMON src/debug.cpp src/timer.cpp)

add_executable(SCoRe ${QUEUE_COMMON} ${SCORE_COMMON} main.cpp)
target_link_libraries(SCoRe ${LIBS})

add_executable(redis_test ${QUEUE_COMMON} tests/redis_test.cpp)
target_link_libraries(redis_test ${LIBS})

add_executable(queue_test ${QUEUE_COMMON} ${DEBUG_COMMON} tests/queue_test.cpp)
target_link_libraries(queue_test ${LIBS})

add_executable(rtqn_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/reverse_triequeue_node_test.cpp)
target_link_libraries(rtqn_test ${LIBS})

add_executable(bench ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} bench.cpp)
target_link_libraries(bench ${LIBS})

add_executable(score_server tests/score_server.cpp ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON})
target_link_libraries(score_server ${LIBS})

#add_executable(main ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} main.cpp)
#target_link_libraries(main ${LIBS})

#These tests seems to rely on the HCL config code
#add_executable(json_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/json_tester.cpp)
#target_link_libraries(json_test ${LIBS})

#This tests doesnt even have stuff
#add_executable(score tests/score.cpp ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON})
#target_link_libraries(score ${LIBS})

