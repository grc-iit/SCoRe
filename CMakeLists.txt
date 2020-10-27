cmake_minimum_required(VERSION 3.10)
project(SCoRe VERSION 1.0.0 DESCRIPTION "")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g")

add_subdirectory(scs_io_common)

include_directories(scs_io_common/include)
include_directories(include)

set(LIBS -lpthread -lmpi -L${CMAKE_BINARY_DIR}/scs_io_common -lscs_io_common)

set(SCORE_COMMON include/common/data_structure.h include/common/enumerations.h
                 include/common/queue/queue.h include/common/queue/queue_config.h
                 include/common/queue/queue_type.h include/common/queue/queue_key.h
                 include/common/queue/redis.h
                 src/common/queue/queue.cpp src/common/queue/queue_config.cpp
                 src/common/queue/queue_type.cpp src/common/queue/queue_key.cpp
                 src/common/queue/redis.cpp)

set(FACT_INCLUDE include/score/factVertex/FactCollector.h include/score/factVertex/FactCollector.h
                 include/score/factVertex/Pythio.h)
set(FACT_DRC src/score/factVertex/FactCollector.cpp src/score/factVertex/FactCollector.cpp
             src/score/factVertex/Pythio.cpp src/score/factVertex/main.cpp)
set(FACT_VERTEX ${SCORE_COMMON} ${FACT_INCLUDE} ${FACT_DRC})
add_executable(factVertex ${FACT_VERTEX})
target_link_libraries(factVertex ${LIBS})

set(INSIGHT_INCLUDE include/score/insightVertex/InsightCollector.h include/score/insightVertex/InsightVertex.h)
set(INSIGHT_SRC src/score/insightVertex/InsightCollector.cpp src/score/insightVertex/InsightVertex.cpp
                src/score/factVertex/Pythio.cpp src/score/insightVertex/main.cpp)

set(INSIGHT_VERTEX ${SCORE_COMMON} ${INSIGHT_INCLUDE} ${INSIGHT_SRC})
add_executable(insightVertex ${INSIGHT_VERTEX})
target_link_libraries(insightVertex ${LIBS})

add_subdirectory(test)