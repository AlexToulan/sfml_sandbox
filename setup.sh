#!/bin/bash

# cmake
rm -rf build

export CC="/usr/bin/clang-17"
export CXX="/usr/bin/clang++-17"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
