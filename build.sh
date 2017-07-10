#!/bin/bash

mkdir -p build
pushd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DSUBMISSION_BUILD=TRUE ..
cmake --build . --target build_submission --clean-first --config Release
popd
