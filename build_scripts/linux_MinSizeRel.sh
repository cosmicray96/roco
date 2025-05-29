#!/bin/bash

set -e

pushd ..

mkdir -p _builds/MinSizeRel
cmake -S . -B _builds/MinSizeRel -DCMAKE_BUILD_TYPE=MinSizeRel "$@"
cmake --build _builds/MinSizeRel

popd
