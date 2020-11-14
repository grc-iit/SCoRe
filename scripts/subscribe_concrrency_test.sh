#!/bin/bash

redis_location='/opt/ohpc/pub/software/hdevarajan/redis-server/6.0.6/bin'
EXEC_PATH='/home/jcernudagarcia/Apollo'

#tcp://127.0.0.1:6379
url="tcp://172.20.101.2:6379"
default_num_threads=31
default_message_size=16
data_per_thread=$(( 256*1024 ))
num_threads=( 1 2 4 8 16 31 )
#num_threads=( 1 )
size_messages=( 1 2 4 8 16 32 )
redis_threads=( 1 2 4 8 16 )

mkdir -p ./results

for redis_thread in "${redis_threads[@]}"
do
  echo "subscribe_redis_thread_test_${redis_thread}"
  ${redis_location}/redis-server "${EXEC_PATH}"/scripts/test_config/redis_"${redis_thread}".conf
  sleep 30
  ${redis_location}/redis-cli ping
  sleep 10
 
  num_messages=$((data_per_thread / default_message_size))

  "${EXEC_PATH}"/build/publish_test 1 "${num_messages}" "${default_message_size}" &
  echo "mpirun -n ${default_num_threads} -f ${EXEC_PATH}/scripts/test_config/hostfile ${EXEC_PATH}/build/queue_test  ${url} 40 ${num_messages} >> ./results/subscribe_redis_thread_test"
  mpirun -n "${default_num_threads}" -f "${EXEC_PATH}"/scripts/test_config/hostfile "${EXEC_PATH}"/build/subscribe_test "${url}" 40 ${num_messages} >> ./results/subscribe_redis_thread_test

  sleep 5
  ${redis_location}/redis-cli flushall
  killall redis-server
  sleep 15
done
