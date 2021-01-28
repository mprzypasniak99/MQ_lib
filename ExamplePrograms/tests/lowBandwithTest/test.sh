#!/bin/bash

tc qdisc add dev lo root tbf rate 10kbps burst 1.5kb limit 10kb

./creator &
./reader

tc qdisc del root dev lo