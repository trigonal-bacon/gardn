#!/usr/bin/bash

set -eu

DEBUG=0
WASM_SERVER=1
TDM=1
GENERAL_SPATIAL_HASH=1
USE_CODEPOINT_LEN=1
SERVER_URL='wss://rysteria.pro/gardn/'
BUILD_TIME=$(date +%s)

git pull --ff-only
cmake -S Client -B Client/build -DDEBUG=$DEBUG -DUSE_CODEPOINT_LEN=$USE_CODEPOINT_LEN -DSERVER_URL=$SERVER_URL -DBUILD_TIME=$BUILD_TIME
make -C Client/build -j$(nproc)
cmake -S Server -B Server/build -DDEBUG=$DEBUG -DWASM_SERVER=$WASM_SERVER -DTDM=$TDM -DGENERAL_SPATIAL_HASH=$GENERAL_SPATIAL_HASH -DUSE_CODEPOINT_LEN=$USE_CODEPOINT_LEN -DBUILD_TIME=$BUILD_TIME
make -C Server/build -j$(nproc)
cd Server/build
until node --inspect gardn-server.js; do sleep 1; done
