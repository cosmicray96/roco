#!/bin/bash

set -e

pushd ..

mkdir -p _builds/Release
cmake -S . -B _builds/Release -DCMAKE_BUILD_TYPE=Release "$@"
cmake --build _builds/Release

popd
