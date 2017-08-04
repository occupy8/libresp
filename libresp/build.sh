#!/bin/bash

set -x

rm -rf build
mkdir build
cd build
cmake -DPACKAGE_LEVEL=lib ../ && make package
cd ../
rm -rf build_devel
mkdir build_devel
cd build_devel
cmake -DPACKAGE_LEVEL=devel ../ && make package