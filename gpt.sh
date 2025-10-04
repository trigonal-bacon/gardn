#!/usr/bin/bash

set -eux

DEBUG=${DEBUG:-0}
WASM_SERVER=${WASM_SERVER:-1}
TDM=${TDM:-0}
GENERAL_SPATIAL_HASH=1
USE_CODEPOINT_LEN=${USE_CODEPOINT_LEN:-1}
WS_URL=${WS_URL:-'ws://localhost:9002'}
VERSION_HASH=${VERSION_HASH:-$(date +%s)}
SERVER_PORT=${SERVER_PORT:-9002}

PULL=${PULL:-1}
JOBS=${JOBS:-1}
PID_FILE='gardn-server.pid'
NGINX_FILE="nginx/$VERSION_HASH.conf"

# Optional: pull latest code
if [[ "$PULL" -eq 1 ]]; then
    git pull --ff-only
fi

# Kill existing process on port 9002 if any
PID_TO_KILL=$(lsof -ti :9002 || true)
if [[ -n "$PID_TO_KILL" ]]; then
    echo "Killing existing process using port 9002 (PID: $PID_TO_KILL)"
    kill -9 $PID_TO_KILL || true
fi

# Start the server
cd Server/build

if [[ "$WASM_SERVER" -eq 1 ]]; then
    node gardn-server.js &
else
    ./gardn-server &
fi

# Wait a moment for server to start
sleep 3
kill -0 $!

# Copy client files
cp ../../Client/public/* ../../Client/build/gardn-client.* . || echo "Warning: client files not found"

# Inject version hash
sed -i "s/const versionHash = '';/const versionHash = '$VERSION_HASH';/" index.html || true

# Reload NGINX config
mkdir -p nginx
echo "'$VERSION_HASH' '$SERVER_PORT';" > "$NGINX_FILE"
echo "default '$SERVER_PORT';" > nginx/default.conf
nginx -s reload

# Signal old process to shut down gracefully (if PID file exists)
if [[ -f "$PID_FILE" ]]; then
    kill -s SIGUSR2 "$(cat "$PID_FILE")" || true
fi

# Save new PID
echo $! > "$PID_FILE"

# Clean up config when process ends
(
    tail --pid $! -f /dev/null
    rm -f "$NGINX_FILE"
    nginx -s reload
) &
