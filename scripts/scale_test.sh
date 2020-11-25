#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo/jarvis/jarvis.py'
CONFIG_PATH='/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini'
LOG_PATH='/home/jcernudagarcia/Apollo/jarvis_log.log'

APOLLO_PATH='/home/jcernudagarcia/Apollo/scripts/test_config/apollo_configs'
EXECUTABLE_PATH='/home/jcernudagarcia/Apollo/build'
RESULT_DIR='/home/jcernudagarcia/Apollo/scripts/results'

num_loops=400
num_threads=40
experiments=( 1_clients 2_clients 4_clients 8_clients 16_clients )
ldms_experiments=( ldms_1_clients ldms_2_clients ldms_4_clients ldms_8_clients ldms_16_clients )
num_clients=( 1 2 4 8 16 )

for i in "${!experiments[@]}"
do
  echo "${experiments[i]}"
  echo "${experiments[i]}" >> ${RESULT_DIR}/real_client_test
  echo "######################### DEPLOY #########################"
  python3 "${JARVIS_PATH}" apollo start "${CONFIG_PATH}" "${experiments[i]}" --log-path ${LOG_PATH} >> /dev/null
  echo "######################### CLIENT #########################"
  mpirun -n "${num_clients[i]}" -f "${APOLLO_PATH}"/client_hostfile "${EXECUTABLE_PATH}"/real_client_test tcp://ares-comp-13:6379 "${num_threads}" "${num_loops}" 0 >> "${RESULT_DIR}"/real_client_test
  echo "######################### CLEAN #########################"
  python3 "${JARVIS_PATH}" apollo stop "${CONFIG_PATH}" "${experiments[i]}" --log-path ${LOG_PATH} >> /dev/null
done

for i in "${!ldms_experiments[@]}"
do
  echo "${experiments[i]}"
  echo  "${experiments[i]}" >> ${RESULT_DIR}/ldms_client_test
  echo "######################### DEPLOY #########################"
  python3 "${JARVIS_PATH}" apollo start "${CONFIG_PATH}" "${ldms_experiments[i]}" --log-path ${LOG_PATH} >> /dev/null
  echo "######################### CLIENT #########################"
  mpirun -n "${num_clients[i]}" -f "${APOLLO_PATH}"/client_hostfile "${EXECUTABLE_PATH}"/real_client_test tcp://ares-comp-13:6379 "${num_threads}" "${num_loops}" 1 >> "${RESULT_DIR}"/ldms_client_test
  echo "######################### CLEAN #########################"
  python3 "${JARVIS_PATH}" apollo stop "${CONFIG_PATH}" "${ldms_experiments[i]}" --log-path ${LOG_PATH} >> /dev/null
done