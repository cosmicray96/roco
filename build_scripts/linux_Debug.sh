#!/bin/bash

set -e

cd "$(dirname "$0")/.." || exit 1

mkdir -p _builds/Debug
cmake -S . -B _builds/Debug -DCMAKE_BUILD_TYPE=Debug "$@" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build _builds/Debug

ln -sf ./_builds/Debug/compile_commands.json .
