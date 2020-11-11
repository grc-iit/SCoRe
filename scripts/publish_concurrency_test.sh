#!/bin/bash

redis_location='/opt/ohpc/pub/software/hdevarajan/redis-server/6.0.6/bin'
EXEC_PATH='/home/jcernudagarcia/Apollo'

default_num_threads=16
default_message_size=16
data_per_thread=$(( 16*1024*1024 ))
num_threads=( 1 2 4 8 16 32 40 )
size_messages=( 32 16 8 4 2 1 )
redis_threads=( 1 2 4 8 16 32 40 )

mkdir -p ./results
for redis_thread in "${redis_threads[@]}"
do
  echo "publish_redis_thread_test_${redis_thread}"
  ${redis_location}/redis-server "${EXEC_PATH}"/scripts/test_config/redis_"${redis_thread}".conf
  sleep 30
  ${redis_location}/redis-cli ping
  sleep 10
  
  num_messages=$((data_per_thread / default_message_size))
  echo "${EXEC_PATH}/build/publish_test ${default_num_threads} ${num_messages} ${default_message_size} >> ./results/publish_redis_thread_test"
  "${EXEC_PATH}"/build/publish_test "${default_num_threads}" ${num_messages} "${default_message_size}" >> ./results/publish_redis_thread_test
  
  sleep 5
  ${redis_location}/redis-cli flushall
  killall redis-server
  sleep 15
done
