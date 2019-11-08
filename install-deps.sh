#!/bin/bash

root=$(pwd)

echo "-----CONAN DEPS-----"
dir="$root/build/conan"
rm -rf $dir
mkdir -p $dir
cd $dir

conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
conan install ../.. --build missing

echo "-----LUA-----"
dir="$root/build/lua"
rm -rf $dir
mkdir -p $dir
cd $dir

wget "https://www.lua.org/ftp/lua-5.3.5.tar.gz"
tar xzf lua*.tar.gz
rm -rf lua*.tar.gz
mkdir bin
cd lua*
make linux local
cp -r "./install/bin" "../bin"
cp -r "./install/include/" "../bin/"