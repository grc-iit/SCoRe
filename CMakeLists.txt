cmake_minimum_required(VERSION 3.15)
project(SCoRe)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(scs_io_common)
include_directories(scs_io_common/include)
include_directories(include)
#set(LIBS -lpthread -lrpc  -lhiredis -lredis++ -lmpi -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common)
set(LIBS -lpthread  -lhiredis -lredis++)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D BENCH_TIMER")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_MSG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_TIMER")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_AUTO_TRACER")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMON_DEBUG_TRACE")

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

set(DEBUG_COMMON include/timer.h src/timer.cpp
        scs_io_common/include/common/debug.h scs_io_common/src/common/debug.cpp
        scs_io_common/include/common/tuple_iter.h)

add_executable(SCoRe ${QUEUE_COMMON} ${SCORE_COMMON} ${DEBUG_COMMON} main.cpp)
#add_dependencies(SCoRe scs_io_common)
target_link_libraries(SCoRe ${LIBS})

#add_executable(client ${QUEUE_COMMON} ${SCORE_COMMON} ${DEBUG_COMMON} client.cpp)
#target_link_libraries(client ${LIBS})
#
#add_executable(redis_test ${QUEUE_COMMON} tests/redis_test.cpp)
#target_link_libraries(redis_test ${LIBS})
#
#add_executable(queue_test ${QUEUE_COMMON} ${DEBUG_COMMON} tests/queue_test.cpp)
#target_link_libraries(queue_test ${LIBS})

add_executable(publish_test ${QUEUE_COMMON} ${DEBUG_COMMON} tests/publish_test.cpp)
target_link_libraries(publish_test ${LIBS} -lmpi)

add_executable(subscribe_test ${QUEUE_COMMON} ${DEBUG_COMMON} tests/subscribe_test.cpp)
target_link_libraries(subscribe_test ${LIBS} -lmpi)

add_executable(subscribe_test_2 ${QUEUE_COMMON} ${DEBUG_COMMON} tests/subscribe_test_2.cpp)
target_link_libraries(subscribe_test_2 ${LIBS} -lmpi)

add_executable(client_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/client_test.cpp)
target_link_libraries(client_test ${LIBS} -lmpi)

add_executable(real_client_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/real_client.cpp)
target_link_libraries(real_client_test ${LIBS} -lmpi)

add_executable(ldms_client_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/ldms_client.cpp)
target_link_libraries(ldms_client_test ${LIBS} -lmpi)

#add_executable(rtqn_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/reverse_triequeue_node_test.cpp)
#target_link_libraries(rtqn_test ${LIBS})
#
#add_executable(bench ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} bench.cpp)
#target_link_libraries(bench ${LIBS})
#
#add_executable(score_server tests/score_server.cpp ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON})
#target_link_libraries(score_server ${LIBS})

#add_executable(main ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} main.cpp)
#target_link_libraries(main ${LIBS})

#These tests seems to rely on the HCL config code
#add_executable(json_test ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON} tests/json_tester.cpp)
#target_link_libraries(json_test ${LIBS})

#This tests doesnt even have stuff
#add_executable(score tests/score.cpp ${QUEUE_COMMON} ${DEBUG_COMMON} ${SCORE_COMMON})
#target_link_libraries(score ${LIBS})

