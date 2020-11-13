#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo/jarvis/jarvis.py'
CONFIG_PATH='/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini'
LOG_PATH='/home/jcernudagarcia/Apollo/jarvis_log.log'

experiments=( nodes_1 nodes_2 nodes_4 nodes_8 nodes_16 )

for experiment in "${experiments[@]}"
do
  echo "${experiment}"
  python3 "${JARVIS_PATH}" apollo start "${CONFIG_PATH}" "${experiment}" --log-path ${LOG_PATH}
done
