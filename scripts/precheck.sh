#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build-precheck"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release -DAGENDA_BUILD_CLI=OFF -DAGENDA_BUILD_GUI=OFF "$ROOT_DIR"
cmake --build . -j $(nproc 2>/dev/null || echo 2)

echo "[OK] Headers compiled successfully (pre-check). Output target: precheck_headers"
