#!/bin/bash

# cmake
rm -rf build

LINK_CLANG_17=False
if $LINK_CLANG_17; then
  ln -s /usr/bin/clang-17 /usr/bin/clang
  ln -s /usr/bin/clang++-17 /usr/bin/clang++
  ln -s /usr/bin/clang-cpp-17 /usr/bin/clang-cpp
  echo "Setting clang++17 to default compiler"
else
  echo "Skipped setting clang-17 and clang++-17 as default clang versions"
fi

export CC="/usr/bin/clang"
export CXX="/usr/bin/clang++"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
