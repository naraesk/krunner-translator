#!/bin/sh
# krunner-translator local backend: manage the llama.cpp server running a
# GGUF translation model (default: tencent/Hy-MT2) on 127.0.0.1:${PORT}.
#
# Usage:
#   ./llama-server.sh start    # start the server if not already running (log: /tmp/krunner-translator-llama.log)
#   ./llama-server.sh stop     # stop the server
#   ./llama-server.sh status   # is it up? (exit 0 = running)
#   ./llama-server.sh test     # translate one line through the HTTP API
#
# The server is shared on its port (default 8080) with other tools that use
# the same local backend. If a llama-server is already answering there,
# start is a no-op.
#
# The KRunner plugin launches `start` automatically on first translation when
# the server is down, so no manual step is needed after boot.
#
# Configuration (all optional; the KRunner config UI sets these for you):
#   MODEL      path to the GGUF model file
#   LLAMA_DIR  directory containing the llama-server binary, searched without
#              recursion: <dir>/llama-server, then <dir>/build/bin/. Leave
#              empty to look for `llama-server` in PATH (system install).
#   PORT       server port (default 8080)
#   NGL        layers offloaded to the GPU (default 99 = all)
#
# A system-wide llama.cpp install (binary in $PATH, libraries in the system
# library path) works without any of the above.
# Empty by default: the binary is looked up in PATH, and the KRunner config
# UI (or the LLAMA_DIR env var) can point at a specific directory.
LLAMA_DIR="${LLAMA_DIR:-}"
# Model selection: override with the MODEL env var, e.g.
# MODEL=/path/to/other-model.gguf ./llama-server.sh start
MODEL="${MODEL:-}"
# Port and GPU layers are overrideable the same way (the KRunner config UI
# writes these two values). PORT defaults to 8080 so it stays in sync with
# tools sharing the local server.
PORT="${PORT:-8080}"
NGL="${NGL:-99}"
LOG=/tmp/krunner-translator-llama.log

is_running() {
    curl -sf --max-time 2 "http://127.0.0.1:${PORT}/health" >/dev/null 2>&1
}

# Locate the llama-server binary inside one directory (no recursion):
#   1. <dir>/llama-server           prebuilt / release drop-in
#   2. <dir>/build/bin/llama-server standard cmake build tree
resolve_in_dir() {
    [ -x "$1/llama-server" ] && { echo "$1/llama-server"; return 0; }
    [ -x "$1/build/bin/llama-server" ] && { echo "$1/build/bin/llama-server"; return 0; }
    return 1
}

# Find the server binary: user-chosen directory first, then $PATH
# (system-wide install).
SERVER_BIN=""
if [ -n "${LLAMA_DIR}" ] && [ -d "${LLAMA_DIR}" ]; then
    SERVER_BIN="$(resolve_in_dir "${LLAMA_DIR}")"
fi
if [ -z "${SERVER_BIN}" ] && command -v llama-server >/dev/null 2>&1; then
    SERVER_BIN="$(command -v llama-server)"
fi

# The model default depends on the llama.cpp directory.
default_model() {
    echo "${LLAMA_DIR}/models/Hy-MT2-1.8B-Q4_K_M.gguf"
}

start() {
    if is_running; then
        echo "llama-server already running on 127.0.0.1:${PORT}"
        return 0
    fi
    if [ -z "${SERVER_BIN}" ]; then
        echo "llama-server binary not found. Set the 'llama.cpp dir' in the" >&2
        echo "KRunner translator settings (or the LLAMA_DIR env var) to a" >&2
        echo "directory containing a llama-server binary (the binary must be" >&2
        echo "release archive or cloned+built checkout); or install" >&2
        echo "llama.cpp into \$PATH." >&2
        return 1
    fi
    [ -n "${MODEL}" ] || MODEL="$(default_model)"
    [ -f "${MODEL}" ] || {
        echo "model not found: ${MODEL}" >&2
        echo "set the 'Model' path in the KRunner translator settings (or the" >&2
        echo "MODEL env var), or download it with:" >&2
        echo "  curl -L -o ${MODEL} https://huggingface.co/tencent/Hy-MT2-1.8B-GGUF/resolve/main/Hy-MT2-1.8B-Q4_K_M.gguf" >&2
        return 1
    }
    # Release/build trees keep the impl shared libs next to the binary;
    # system installs find their libraries through the normal loader.
    BIN_DIR="$(dirname "${SERVER_BIN}")"
    LD_LIBRARY_PATH="${BIN_DIR}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
    export LD_LIBRARY_PATH
    # Fail fast on a broken binary (e.g. missing shared libraries) instead of
    # after the health poll below.
    if ! "${SERVER_BIN}" --version >/dev/null 2>&1; then
        echo "llama-server failed to run: ${SERVER_BIN}" >&2
        echo "check that its shared libraries are available (point LLAMA_DIR" >&2
        echo "at the directory the binary was built in), or see ${LOG}" >&2
        return 1
    fi
    nohup "${SERVER_BIN}" \
        -m "${MODEL}" --jinja -ngl "${NGL}" -c 2048 --cache-ram 0 --port "${PORT}" --host 127.0.0.1 \
        > "${LOG}" 2>&1 &
    echo "llama-server starting (pid $!)... log: ${LOG}"
    # Wait until the HTTP endpoint answers (model load takes a few seconds).
    for i in 1 2 3 4 5 6 7 8 9 10; do
        if curl -sf "http://127.0.0.1:${PORT}/health" >/dev/null 2>&1; then
            echo "ready: http://127.0.0.1:${PORT}"
            return 0
        fi
        sleep 1
    done
    echo "server did not become ready; see ${LOG}" >&2
    return 1
}

stop() {
    if is_running; then
        pkill -f "llama-server.*${PORT}"
        echo "stopped"
    else
        echo "not running"
    fi
}

status() {
    if is_running; then
        echo "running (pid $(pgrep -f "llama-server.*${PORT}" | head -1))"
        return 0
    fi
    echo "not running"
    return 1
}

test_translate() {
    if ! is_running; then
        echo "server not running, start it first: ./llama-server.sh start" >&2
    fi
    curl -s "http://127.0.0.1:${PORT}/v1/chat/completions" \
        -H "Content-Type: application/json" \
        -d '{"messages":[{"role":"user","content":"Translate the following text into Chinese. Note that you should only output the translated result without any additional explanation:\n\nThe quick brown fox jumps over the lazy dog."}],"temperature":0.7,"top_p":0.6,"top_k":20,"repeat_penalty":1.05,"max_tokens":4096}' \
    | python3 -c "import json,sys; print(json.load(sys.stdin)['choices'][0]['message']['content'].strip())"
}

case "${1:-}" in
    start)  start ;;
    stop)   stop ;;
    status) status ;;
    test)   test_translate ;;
    *)      echo "usage: $0 {start|stop|status|test}"; exit 1 ;;
esac
