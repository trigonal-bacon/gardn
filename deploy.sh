#!/usr/bin/bash

set -eux

DEBUG=${DEBUG:-0}
WASM_SERVER=${WASM_SERVER:-1}
TDM=${TDM:-0}
GENERAL_SPATIAL_HASH=1
USE_CODEPOINT_LEN=${USE_CODEPOINT_LEN:-1}
WS_URL=${WS_URL:-'wss://gardn.pro/ws/'}
VERSION_HASH=${VERSION_HASH:-$(date +%s)}
SERVER_PORT=${SERVER_PORT:-$(python3 -c 'import socket; s = socket.socket(); s.bind(("", 0)); print(s.getsockname()[1]); s.close()')}

PULL=${PULL:-1}
JOBS=${JOBS:-1}
PID_FILE='gardn-server.pid'
NGINX_FILE="nginx/$VERSION_HASH.conf"

# if [[ "$PULL" -eq 1 ]]; then
#     git pull --ff-only
# fi

cmake -S Client -B Client/build \
    "-DDEBUG=$DEBUG" \
    "-DUSE_CODEPOINT_LEN=$USE_CODEPOINT_LEN" \
    "-DWS_URL=$WS_URL" \
    "-DVERSION_HASH=$VERSION_HASH"
make -C Client/build "-j$JOBS"

cmake -S Server -B Server/build \
    "-DDEBUG=$DEBUG" \
    "-DWASM_SERVER=$WASM_SERVER" \
    "-DTDM=$TDM" \
    "-DGENERAL_SPATIAL_HASH=$GENERAL_SPATIAL_HASH" \
    "-DUSE_CODEPOINT_LEN=$USE_CODEPOINT_LEN" \
    "-DVERSION_HASH=$VERSION_HASH" \
    "-DSERVER_PORT=$SERVER_PORT"
make -C Server/build "-j$JOBS" 

cd Server/build

if [[ "$WASM_SERVER" -eq 1 ]]; then
    node gardn-server.js &
else
    ./gardn-server &
fi

sleep 3
kill -0 $!

cp ../../Client/public/* ../../Client/build/gardn-client.* .
sed -i "s/const versionHash = '';/const versionHash = '$VERSION_HASH';/" index.html

mkdir -p nginx
echo "'$VERSION_HASH' '$SERVER_PORT';" > "$NGINX_FILE"
echo "default '$SERVER_PORT';" > nginx/default.conf
nginx -s reload

if [[ -f "$PID_FILE" ]]; then
    kill -s SIGUSR2 "$(cat "$PID_FILE")" || true
fi
echo $! > "$PID_FILE"

(
    tail --pid $! -f /dev/null
    rm -f "$NGINX_FILE"
    nginx -s reload
) &
