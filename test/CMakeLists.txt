cmake_minimum_required(VERSION 3.10)
project(score_test)
set(CMAKE_CXX_STANDARD 17)

#set(LIBS -lpthread -lmpi -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common -lhiredis -lredis++)
set(LIBS -lpthread -lmpi -lhiredis -lredis++)

add_executable(queue_test common/queue/queue_test.cpp )
add_dependencies(queue_test score)
target_link_libraries(queue_test score ${LIBS})