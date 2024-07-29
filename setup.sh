#!/bin/bash

# clean
rm -rf build
mkdir build

# set clang version
export CC="/usr/bin/clang-17"
export CXX="/usr/bin/clang++-17"
export CMAKE_GENERATOR=Ninja

# cmake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j10
