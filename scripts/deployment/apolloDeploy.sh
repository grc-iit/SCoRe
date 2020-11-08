#!/bin/bash

keys=()
values=()
while IFS=' ' read -r key value
do
    keys+=("$key")
    values+=("$value")
done < key_value_pairs.txt

for node in "${fact_vertex[@]}"
do
ssh ${node} /bin/bash << EOF
redis-server --daemonize yes
../../build/SCoRe
EOF
done

for node in "${insight_vertex[@]}"
do
ssh ${node} /bin/bash << EOF
redis-server --daemonize yes
../../build/SCoRe
EOF
done