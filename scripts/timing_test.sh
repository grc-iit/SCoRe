#!/bin/bash

EXEC_PATH='/home/jcernudagarcia/Apollo'
redis_location='/opt/ohpc/pub/software/hdevarajan/redis-server/6.0.6/bin'

${redis_location}/redis-server "${EXEC_PATH}"/scripts/test_config/redis_1.conf
sleep 5
${redis_location}/redis-cli ping

mdir -p ./results
"${EXEC_PATH}"/build/SCoRe "${HOME}"/Apollo/configs/config_2_1.json 1 >> ./results/timming_fact_vertex
sleep 5
"${EXEC_PATH}"/build/SCoRe "${EXEC_PATH}"/configs/config_2_1.json 0 >> ./results/timming_insight_vertex

${redis_location}/redis-cli flushall
killall redis-server