#!/bin/bash

redis_location='/opt/ohpc/pub/software/hdevarajan/redis-server/6.0.6/bin'
EXEC_PATH='/home/jcernudagarcia/Apollo'

default_num_threads=16
default_message_size=16
data_per_thread=$(( 16*1024*1024 ))
num_threads=( 1 2 4 8 16 32 40 )
size_messages=( 1 2 4 8 16 32 )
redis_threads=( 1 2 4 8 16 32 40)

${redis_location}/redis-server "${EXEC_PATH}"/scripts/test_config/redis_1.conf
sleep 5
${redis_location}/redis-cli ping

mkdir -p ./results
for thread in "${num_threads[@]}"
do
  echo "publish_thread_test_${thread}"
  num_messages=$((data_per_thread / default_message_size))
  echo "${EXEC_PATH}/build/queue_test ${thread} ${num_messages} ${default_message_size} >> ./results/publish_thread_test_${thread}"
  "${EXEC_PATH}"/build/queue_test "${thread}" "${num_messages}" "${default_message_size}" >> ./results/publish_thread_test_"${thread}"
  sleep 5
  ${redis_location}/redis-cli flushall
done

for size_message in "${size_messages[@]}"
do
  echo "publish_size_test_${size_message}"
  num_messages=$((data_per_thread / size_message))
  echo "${EXEC_PATH}/build/queue_test ${default_num_threads} ${num_messages} ${size_message} >> ./results/publish_size_test_${size_message}"
  "${EXEC_PATH}"/build/queue_test "${default_num_threads}" "${num_messages}" "${size_message}" >> ./results/publish_size_test_"${size_message}"
  sleep 5
  ${redis_location}/redis-cli flushall
done

for redis_thread in "${redis_threads[@]}"
do
  echo "publish_redis_thread_test_${redis_thread}"
  killall redis-server
  ${redis_location}/redis-server "${EXEC_PATH}"/scripts/test_config/redis_"${redis_thread}".conf
  num_messages=$((data_per_thread / default_message_size))
  echo "${EXEC_PATH}/build/queue_test ${default_num_threads} ${num_messages} ${default_message_size} >> ./results/publish_redis_thread_test_${redis_thread}"
  "${EXEC_PATH}"/build/queue_test "${default_num_threads}" ${num_messages} "${default_message_size}" >> ./results/publish_redis_thread_test_"${redis_thread}"
  sleep 5
  ${redis_location}/redis-cli flushall
done

killall redis-server