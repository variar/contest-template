#!/usr/bin/env bash

mkdir -p build
pushd build

cmake -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DSUBMISSION_BUILD=TRUE \
        -DCMAKE_C_COMPILER=clang-4.0 \
        -DCMAKE_CXX_COMPILER=clang++-4.0 ..

cmake --build . --target build_submission --clean-first --config Release

popd
