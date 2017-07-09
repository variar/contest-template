#!/bin/bash

mkdir -p build
pushd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DSUBMISSION_BUILD=TRUE ..
cmake --build .
popd
