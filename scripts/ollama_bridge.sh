#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
PY=${PYTHON:-python3}
exec "$PY" -m ai.jarvis.bridge
