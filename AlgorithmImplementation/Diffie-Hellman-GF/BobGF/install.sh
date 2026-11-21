#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

cp "$SCRIPT_DIR/build-Release/bin/BobGF" \
   "$PROJECT_ROOT/usr/resource/BobGF"
