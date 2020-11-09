#!/bin/bash

JARVIS_PATH='/home/jcernudagarcia/Apollo'
CONFIG_PATH='/home/jcernudagarcia/Apollo'

experiments=( sd dasd )

for experiment in "${experiments[@]}"
do
  echo "${experiment}"
  "${JARVIS_PATH}"/jarvis.py apollo start "${CONFIG_PATH}" "${experiment}"
done