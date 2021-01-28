#!/bin/bash

./reader &
sleep 1s

for i in {1..10}
do
    ./creator "user$1" "password" &
    sleep 2
done