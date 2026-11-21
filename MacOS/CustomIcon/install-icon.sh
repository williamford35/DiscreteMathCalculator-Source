#!/bin/bash
set -e

APP="$1"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

cp -p "$SCRIPT_DIR/"$'Icon\r' "$APP/"

FINDER_INFO="$(cat "$SCRIPT_DIR/FinderInfo.hex")"

xattr -wx com.apple.FinderInfo "$FINDER_INFO" "$APP"

touch "$APP"
