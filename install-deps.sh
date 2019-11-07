#!/bin/bash

dir="./build/conan"

rm -rf $dir
mkdir -p $dir
cd $dir

conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
conan install ../.. --build missing