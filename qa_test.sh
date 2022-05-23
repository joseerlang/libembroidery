#!/bin/bash

function c_build () {
rm -fr build
mkdir build
cd build
cmake ..
make 2> build_error.log
./embroider --test
cd ..
}

python3 -m build
python3 -m pip install dist/*.whl 
python3 tests.py
