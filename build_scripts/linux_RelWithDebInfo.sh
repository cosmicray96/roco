#!/bin/bash

set -e

pushd ..

mkdir -p _builds/RelWithDebInfo
cmake -S . -B _builds/RelWithDebInfo -DCMAKE_BUILD_TYPE=RelWithDebInfo "$@"
cmake --build _builds/RelWithDebInfo

popd
