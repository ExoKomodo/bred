#! /bin/bash

set -e

cd $(dirname $BASH_SOURCE[0])

export C_INCLUDE_PATH=./src/c:${C_INCLUDE_PATH}
DAYBREAK=./bin/daybreak

BUILD_DIR=./out/$(uname)
mkdir -p ${BUILD_DIR}
${DAYBREAK} ./src/main.day -o ${BUILD_DIR}/ded
