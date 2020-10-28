cmake_minimum_required(VERSION 3.15)
project(SCoRe)

set(CMAKE_CXX_STANDARD 17)

#include_directories(scs_io_common/include)
include_directories(include)
set(LIBS -lpthread -lrpc  -lhiredis -lredis++ -lmpi -luv)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D BENCH_TIMER")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

set(QUEUE_COMMON src/queue.cpp src/queue_key.cpp src/queue_type.cpp src/mon_hooks.cpp
        include/redis.h src/redis.cpp include/queue_config.h src/queue_config.cpp)


add_executable(SCoRe ${QUEUE_COMMON} include/reverse_trie_queue_node_config.h src/reverse_trie_queue_node_config.cpp
                     include/config_generator.h src/config_generator.cpp
                     include/SCoRe.h src/SCoRe.cpp)
target_link_libraries(SCoRe ${LIBS})

add_executable(redis_test ${QUEUE_COMMON} tests/redis_test.cpp)
target_link_libraries(redis_test ${LIBS})

add_executable(queue_test ${QUEUE_COMMON} tests/queue_test.cpp src/debug.cpp src/timer.cpp  src/s_queues.cpp)
target_link_libraries(queue_test ${LIBS})

add_executable(rtqn_test ${QUEUE_COMMON} tests/reverse_triequeue_node_test.cpp src/reverse_trie_queue_node_key.cpp
               src/reverse_trie_queue_node.cpp src/reverse_trie_queue_node_config.cpp
               src/timer.cpp src/s_queues.cpp
               src/config_generator.cpp)
target_link_libraries(rtqn_test ${LIBS})

add_executable(bench ${QUEUE_COMMON} bench.cpp src/SCoRe.cpp src/timer.cpp src/reverse_trie_queue.cpp src/reverse_trie_queue_node_key.cpp
               src/reverse_trie_queue_node.cpp src/reverse_trie_queue_node_config.cpp
                 src/s_queues.cpp
               src/config_generator.cpp)
target_link_libraries(bench ${LIBS})

add_executable(main ${QUEUE_COMMON} main.cpp
        src/SCoRe.cpp
        src/reverse_trie_queue.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        src/s_queues.cpp
        src/config_generator.cpp
        )
target_link_libraries(main ${LIBS})

add_executable(json_test tests/json_tester.cpp include/reverse_trie_queue_node.h)
target_link_libraries(json_test ${LIBS})

add_executable(score_server tests/score_server.cpp src/reverse_trie_queue_node_key.cpp src/reverse_trie_queue_node.cpp
               src/reverse_trie_queue_node_config.cpp
               src/s_queues.cpp src/config_generator.cpp
               src/SCoRe.cpp)
target_link_libraries(score_server ${LIBS})

add_executable(score tests/score.cpp src/reverse_trie_queue_node_key.cpp src/reverse_trie_queue_node.cpp
               src/reverse_trie_queue_node_config.cpp
               src/s_queues.cpp src/config_generator.cpp
               src/SCoRe.cpp)
target_link_libraries(score ${LIBS})

