#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

cp "$SCRIPT_DIR/build/qs" "$PROJECT_ROOT/usr/resource/qs"

echo "qs installed successfully."
