#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON_WRAPPER_DIR="$SCRIPT_DIR/python3-venv-wrapper"
IDF_VERSION="${IDF_VERSION:-v5.5.1}"
IDF_TARGET="${IDF_TARGET:-esp32c6}"

if ! command -v eim >/dev/null 2>&1; then
    printf 'eim was not found in PATH. Install Espressif Installation Manager first.\n' >&2
    exit 1
fi

if [ -n "${ESP_IDF_PROXY:-}" ]; then
    export HTTP_PROXY="$ESP_IDF_PROXY"
    export HTTPS_PROXY="$ESP_IDF_PROXY"
    export ALL_PROXY="$ESP_IDF_PROXY"
    export http_proxy="$ESP_IDF_PROXY"
    export https_proxy="$ESP_IDF_PROXY"
    export all_proxy="$ESP_IDF_PROXY"
fi

PATH="$PYTHON_WRAPPER_DIR:$PATH" eim install \
    -i "$IDF_VERSION" \
    -t "$IDF_TARGET" \
    -n true \
    --do-not-track true \
    --cleanup true \
    "$@"
