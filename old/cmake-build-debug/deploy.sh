#! /usr/bin/env bash 


./main ../configs/config_1.json 1 & 
$! > node_1.pid
./main ../configs/config_1.json 2 &
$! > node_2.pid
./main ../configs/config_1.json 0 &
$! > node_0.pid
