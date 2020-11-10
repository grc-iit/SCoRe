#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo/jarvis/jarvis.py'
CONFIG_PATH='/home/jcernudagarcia/Apollo/jarvis/repos/apollo/default.ini'

experiments=( localhost_2 )

for experiment in "${experiments[@]}"
do
  echo "${experiment}"
  python "${JARVIS_PATH}"/jarvis.py apollo start "${CONFIG_PATH}" "${experiment}"
done