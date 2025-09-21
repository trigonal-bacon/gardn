#!/usr/bin/bash

set -eux

PULL=${PULL:-1}
JOBS=${JOBS:-1}
PID_FILE='gardn-server.pid'

DEBUG=${DEBUG:-0}
WASM_SERVER=${WASM_SERVER:-1}
TDM=${TDM:-1}
GENERAL_SPATIAL_HASH=1
USE_CODEPOINT_LEN=${USE_CODEPOINT_LEN:-1}
WS_URL=${WS_URL:-'wss://rysteria.pro/gardn/'}
VERSION_HASH=${VERSION_HASH:-$(date +%s)}
SERVER_PORT=${SERVER_PORT:-$(python -c 'import socket; s = socket.socket(); s.bind(("", 0)); print(s.getsockname()[1]); s.close()')}

if [[ "$PULL" == 1 ]]; then
    git pull --ff-only
fi

cmake -S Client -B Client/build \
    -DDEBUG="$DEBUG" \
    -DUSE_CODEPOINT_LEN="$USE_CODEPOINT_LEN" \
    -DWS_URL="$WS_URL" \
    -DVERSION_HASH="$VERSION_HASH"
make -C Client/build -j"$JOBS"

cmake -S Server -B Server/build \
    -DDEBUG="$DEBUG" \
    -DWASM_SERVER="$WASM_SERVER" \
    -DTDM="$TDM" \
    -DGENERAL_SPATIAL_HASH="$GENERAL_SPATIAL_HASH" \
    -DUSE_CODEPOINT_LEN="$USE_CODEPOINT_LEN" \
    -DVERSION_HASH="$VERSION_HASH" \
    -DSERVER_PORT="$SERVER_PORT"
make -C Server/build -j"$JOBS"

cd Server/build
if [[ "$WASM_SERVER" == 1 ]]; then
    node gardn-server.js &
else
    ./gardn-server &
fi
sleep 1
kill -0 $!

cd -
cp Client/build/gardn-client.* Client/public
cp Client/public/* Server/build

cd Server/build
echo "set \$port $SERVER_PORT;" > nginx.conf
nginx -s reload

if [[ -f "$PID_FILE" ]]; then
    kill -s SIGUSR2 $(cat "$PID_FILE") || true
fi
echo $! > "$PID_FILE"
