#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo/jarvis/jarvis.py'
CONFIG_PATH='/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini'
LOG_PATH='/home/jcernudagarcia/Apollo/jarvis_log.log'

APOLLO_PATH='/home/jcernudagarcia/Apollo/scripts/test_config/apollo_configs'
EXECUTABLE_PATH='/home/jcernudagarcia/Apollo/build'
RESULT_DIR='/home/jcernudagarcia/Apollo/scripts/results'

experiment=16_clients
num_clients=1

num_loops=( 1 2 4 8 16 32 64 128 256 1024 2048 4096)

python3 "${JARVIS_PATH}" apollo start "${CONFIG_PATH}" "${experiment}" --log-path ${LOG_PATH}

for i in "${!num_loops[@]}"
do
  echo "Loops: ${num_loops[i]}" >> "${RESULT_DIR}"/scale_timming_test
  mpirun -n "${num_clients}" -f "${APOLLO_PATH}"/client_hostfile "${EXECUTABLE_PATH}"/real_client_test tcp://ares-comp-13:6380 40 "${num_loops[i]}" >> "${RESULT_DIR}"/scale_timming_test
  python3 "${JARVIS_PATH}" apollo clean "${CONFIG_PATH}" "${experiment}" --log-path ${LOG_PATH}
done

python3 "${JARVIS_PATH}" apollo stop "${CONFIG_PATH}" "${experiment}" --log-path ${LOG_PATH}
