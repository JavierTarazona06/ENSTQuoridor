#!/usr/bin/env bash
set -euo pipefail
CONFIG="${1:-Debug}"

# Resolve repo root as script dir/.. and cd there
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Ensure VCPKG_ROOT
if [[ -z "${VCPKG_ROOT:-}" ]]; then
  if [[ ! -d .vcpkg ]]; then
    echo "Cloning vcpkg locally into .vcpkg ..."
    git clone https://github.com/microsoft/vcpkg.git .vcpkg >/dev/null
    (cd .vcpkg && ./bootstrap-vcpkg.sh >/dev/null)
  fi
  export VCPKG_ROOT="$(cd .vcpkg && pwd)"
fi

echo "Using VCPKG_ROOT=$VCPKG_ROOT"

# Detect platform preset
UNAME_OUT="$(uname -s)"
case "$UNAME_OUT" in
  Linux*)  PRESET="x64-linux" ;;
  Darwin*) PRESET="arm64-osx" ;;
  *) echo "Unsupported platform: $UNAME_OUT"; exit 1 ;;
 esac

# Configure and build
cmake --preset "$PRESET"
cmake --build --preset "$PRESET" --config "$CONFIG"
