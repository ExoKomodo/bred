#! /bin/bash

set -e

cd $(dirname $BASH_SOURCE[0])

export C_INCLUDE_PATH="./daybreak/src/include:${C_INCLUDE_PATH}"
gcc ./bin/main.day.c -o ./bin/daybreak
