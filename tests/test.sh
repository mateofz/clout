#!/bin/sh

set -e

echo "test: exit status 0"
./clout sh -c 'exit 0'
test $? -eq 0

echo "test: exit status 42"
./clout sh -c 'exit 42'
test $? -eq 42

echo "test: signal termination"
./clout sh -c 'kill -TERM $$'
test $? -eq 143

echo "test: stderr + exit status"
./clout sh -c 'echo hello >&2; exit 42'
test $? -eq 42

echo "test: invalid option"
if ./clout -x >/dev/null 2>&1; then
    echo "FAIL: invalid option returned 0"
    exit 1
fi

echo "test: help"
./clout -h >/dev/null 2>&1

echo "all tests passed"
