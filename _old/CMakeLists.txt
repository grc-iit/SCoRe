cmake_minimum_required(VERSION 3.15)
project(SCoRe)

file(GLOB SOURCES
        include/*.h
        src/*.cpp
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES -lhiredis )
set(CMAKE_CXX_STANDARD_LIBRARIES -lredis++ )
set(CMAKE_CXX_STANDARD_LIBRARIES -pthread )
set(CMAKE_CXX_FLAGS -fpermissive)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D BENCH_TIMER")
#set(CMAKE_CXX_FLAGS_DEBUG "-D BENCH_TIMER -D_DEBUG")

#set(CMAKE_CXX_FLAGS lhiredis -pthread)
#set(CMAKE_EXE_LINKER_FLAGS lhiredis -pthread)
add_library(hiredis SHARED IMPORTED ../src/common/queue/Queue.cpp ../src/common/queue/redis.cpp queue_type.cpp) # or STATIC instead of SHARED
add_library(pthread SHARED IMPORTED) # or STATIC instead of SHARED
add_library(redis++ SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(redis++ PROPERTIES
        IMPORTED_LOCATION "/home/neeraj/root/lib/libredis++.so"
        )
set_target_properties(hiredis PROPERTIES
        IMPORTED_LOCATION "/usr/lib64/libhiredis.so"
        )

include_directories("include/" "/home/neeraj/root/include/" "/home/neeraj/root/usr/local/include")
link_directories("/home/neeraj/root/lib/" "/home/neeraj/root/usr/local/lib" "/usr/lib64/")

add_executable(SCoRe ${SOURCES} include/reverse_trie_queue_node_config.h src/reverse_trie_queue_node_config.cpp queue_config.h queue_config.cpp src/config_generator.cpp src/SCoRe.cpp)
#[[

add_executable(redis.o src/redis.cpp)
target_link_libraries(redis.o hiredis)
target_link_libraries(redis.o redis++)

]]

add_executable(redis_test ../test/common/queue/redis_test.cpp ../src/common/queue/redis.cpp queue_config.cpp)
target_link_libraries(redis_test hiredis)
target_link_libraries(redis_test redis++)


add_executable(queue_test tests/queue_test.cpp
        ../src/common/queue/Queue.cpp
        queue_key.cpp queue_type.cpp
        src/debug.cpp
        src/timer.cpp
        ../src/common/queue/redis.cpp src/s_queues.cpp queue_config.h queue_config.cpp)
target_link_libraries(queue_test hiredis)
target_link_libraries(queue_test redis++)
target_link_libraries(queue_test mpi)

add_executable(rtqn_test tests/reverse_triequeue_node_test.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        ../src/common/queue/Queue.cpp
        src/timer.cpp
        queue_key.cpp
        src/mon_hooks.cpp
        src/s_queues.cpp
        queue_type.cpp
        ../src/common/queue/redis.cpp
        queue_config.cpp
        src/config_generator.cpp)
target_link_libraries(rtqn_test hiredis)
target_link_libraries(rtqn_test redis++)

#
#add_executable(rtq_test tests/reverse_trie_queue_test.cpp include/reverse_trie_queue_node_config.h src/reverse_trie_queue_node_config.cpp include/queue_config.h src/queue_config.cpp src/config_generator.cpp)
#target_link_libraries(rtq_test hiredis)
#target_link_libraries(rtq_test redis++)
#

add_executable(bench bench.cpp
        src/SCoRe.cpp
        src/timer.cpp
        src/reverse_trie_queue.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        ../src/common/queue/Queue.cpp
        queue_key.cpp
        src/mon_hooks.cpp
        src/s_queues.cpp
        queue_type.cpp
        ../src/common/queue/redis.cpp
        queue_config.cpp
        src/config_generator.cpp
        )
target_link_libraries(bench hiredis)
target_link_libraries(bench redis++)
target_link_libraries(bench rpc)



add_executable(main main.cpp
        src/SCoRe.cpp
        src/reverse_trie_queue.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        ../src/common/queue/Queue.cpp
        queue_key.cpp
        src/mon_hooks.cpp
        src/s_queues.cpp
        queue_type.cpp
        ../src/common/queue/redis.cpp
        queue_config.cpp
        src/config_generator.cpp
        )
target_link_libraries(main hiredis)
target_link_libraries(main redis++)
target_link_libraries(main rpc)


#add_executable(main main.cpp
#        src/reverse_trie_queue_node_key.cpp
#        src/reverse_trie_queue_node.cpp
#        src/reverse_trie_queue_node_config.cpp
#        src/queue.cpp
#        src/queue_key.cpp
#        src/mon_hooks.cpp
#        src/s_queues.cpp
#        src/queue_type.cpp
#        src/redis.cpp
#        src/queue_config.cpp
#        src/config_generator.cpp)
#target_link_libraries(main hiredis)
#target_link_libraries(main redis++)

add_executable(json_test tests/json_tester.cpp
        include/reverse_trie_queue_node.h)
#target_link_libraries(json_test reverse_trie_queue_node.o)



add_executable(score_server tests/score_server.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        ../src/common/queue/Queue.cpp
        queue_key.cpp
        src/mon_hooks.cpp
        src/s_queues.cpp
        queue_type.cpp
        ../src/common/queue/redis.cpp
        queue_config.cpp
        src/config_generator.cpp
        src/SCoRe.cpp
        )
target_link_libraries(score_server hiredis)
target_link_libraries(score_server redis++)

add_executable(score tests/score.cpp
        src/reverse_trie_queue_node_key.cpp
        src/reverse_trie_queue_node.cpp
        src/reverse_trie_queue_node_config.cpp
        ../src/common/queue/Queue.cpp
        queue_key.cpp
        src/mon_hooks.cpp
        src/s_queues.cpp
        queue_type.cpp
        ../src/common/queue/redis.cpp
        queue_config.cpp
        src/config_generator.cpp
        src/SCoRe.cpp
        )
target_link_libraries(score hiredis)
target_link_libraries(score redis++)

