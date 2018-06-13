#!/usr/bin/env bash

mkdir -p build-dev
pushd build-dev

cmake -G "Ninja" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DSUBMISSION_BUILD=FALSE \
        -DCMAKE_C_COMPILER=clang-5.0 \
        -DCMAKE_CXX_COMPILER=clang++-5.0 ..

cmake --build . --clean-first --config Debug

popd
