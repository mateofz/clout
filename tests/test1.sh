#!/usr/bin/env bash

for i in $(seq 1 10); do
    if (( RANDOM % 2 )); then
        echo "[$i] stdout: line $i"
    else
        echo "[$i] stderr: line $i" >&2
    fi
    sleep 0.1
done
