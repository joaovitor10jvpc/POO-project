#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${ROOT}/build"

mkdir -p "${BUILD}"
cd "${BUILD}"
cmake .. -DAGENDA_BUILD_CLI=ON
cmake --build . -j

echo "== DEMO =="
./agenda_cli --demo || true

echo "== LIST DAY =="
./agenda_cli view-day --date 2025-09-25 || true

echo "== SAVE / LOAD =="
./agenda_cli add --title Demo --date 2025-10-01 --time 08:00 --dur 30 --owner 1 --tags teste || true
./agenda_cli save --file demo.json || true
./agenda_cli load --file demo.json || true
./agenda_cli list || true
