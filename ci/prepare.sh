#!/bin/bash
set -e

mv ./ci/__private ../__private
./qtmodules-travis/ci/linux/build-all-orig.sh
