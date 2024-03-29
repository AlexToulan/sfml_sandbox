#!/bin/bash

# clean
rm -rf build
mkdir build

# set clang version
export CC="/usr/bin/clang-17"
export CXX="/usr/bin/clang++-17"

# cmake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
