#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo/jarvis/jarvis.py'
CONFIG_PATH='/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini'
LOG_PATH='/home/jcernudagarcia/Apollo/jarvis_log.log'

experiments=( nodes_1 nodes_2 nodes_4 nodes_8 nodes_16 )
APOLLO_PATH='/home/jcernudagarcia/Apollo/scripts/test_config/apollo_configs'
EXECUTABLE_PATH='/home/jcernudagarcia/Apollo/build'
RESULT_DIR='/home/jcernudagarcia/Apollo/scripts/results'

experiments=( 2_clients )
num_clients=( 2 )

for i in "${!experiments[@]}"
do
  echo "${experiments[i]}"
  python3 "${JARVIS_PATH}" apollo start "${CONFIG_PATH}" "${experiments[i]}" --log-path ${LOG_PATH}
  mpirun -n "${num_clients[i]}" -f "${APOLLO_PATH}"/client_hostfile "${EXECUTABLE_PATH}"/real_client_test tcp://ares-comp-13:6379 >> "${RESULT_DIR}"/real_client-results
  python3 "${JARVIS_PATH}" apollo stop "${CONFIG_PATH}" "${experiments[i]}" --log-path ${LOG_PATH}
done
