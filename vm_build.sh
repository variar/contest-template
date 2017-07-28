#!/usr/bin/env bash
SSH_OPTS="-o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no"

mkdir -p build_clean_sources
pushd build_clean_sources

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DSUBMISSION_BUILD=TRUE ..
cmake --build . --target pack_clean_sources --clean-first --config Release

cat submission_source.tar.gz | ssh ${SSH_OPTS} $1 \
    'cat > submission_source.tar.gz \
        && rm -rf submission && mkdir submission \
        && cd submission \
        && tar -xzf ../submission_source.tar.gz > /dev/null 2>&1 \
        && ./build.sh > /dev/null 2>&1 && cmake --build ./build --target pack_submission > /dev/null 2>&1 \
        && cat ./build/submission.tar.gz' > vm_submission.tar.gz

popd