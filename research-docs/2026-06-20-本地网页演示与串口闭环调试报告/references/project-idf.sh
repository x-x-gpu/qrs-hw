#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IDF_ACTIVATE="${IDF_ACTIVATE:-$HOME/.espressif/tools/activate_idf_v5.5.1.sh}"

if [ ! -f "$IDF_ACTIVATE" ]; then
    printf 'ESP-IDF activation script was not found: %s\n' "$IDF_ACTIVATE" >&2
    printf 'Run tools/install-esp-idf-linux.sh first, or set IDF_ACTIVATE to the correct activation script.\n' >&2
    exit 1
fi

while IFS='=' read -r key value; do
    case "$key" in
        PATH|IDF_TOOLS_PATH|IDF_COMPONENT_LOCAL_STORAGE_URL|IDF_PATH|ESP_ROM_ELF_DIR|OPENOCD_SCRIPTS|IDF_PYTHON_ENV_PATH|ESP_IDF_VERSION)
            export "$key=$value"
            ;;
    esac
done < <("$IDF_ACTIVATE" -e)

prepend_path_if_dir() {
    if [ -d "$1" ]; then
        export PATH="$1:$PATH"
    fi
}

# Prefer ESP-IDF-managed tools over distro packages when both are present.
prepend_path_if_dir "$HOME/.espressif/tools/cmake/3.30.2/bin"
prepend_path_if_dir "$HOME/.espressif/tools/ninja/1.12.1"

export IDF_TARGET="${IDF_TARGET:-esp32c6}"

cd "$PROJECT_ROOT"
"$IDF_PYTHON_ENV_PATH/bin/python" "$IDF_PATH/tools/idf.py" "$@"
