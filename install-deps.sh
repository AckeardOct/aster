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
mkdir -p bin/include/lua
cd lua*
make linux local
cp -r ./install/lib/*.a  ../bin/
cp -r ./install/bin/*    ../bin/
cp -r ./install/include/* ../bin/include/lua/

luapri="$dir/lua.pri"
touch $luapri
echo "LIBS += -L\$\$PWD/bin/ -llua"           >> $luapri
echo "INCLUDEPATH += \$\$PWD/bin/include"     >> $luapri
echo "DEPENDPATH += \$\$PWD/bin/include"      >> $luapri
echo "PRE_TARGETDEPS += \$\$PWD/bin/liblua.a" >> $luapri

cd $root
echo "----- GOOGLE TESTS -----"
dir="$root/build/gtest"
rm -rf $dir
mkdir -p $dir
cd $dir
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
cd $dir

gtestpri="$dir/gtest.pri"
touch $gtestpri
echo "LIBS += -L\$\$PWD/googletest/build/lib/ -lgtest"           >> $gtestpri
echo "INCLUDEPATH += \$\$PWD/googletest/googletest/include/"     >> $gtestpri
echo "DEPENDPATH += \$\$PWD/googletest/googletest/include/"      >> $gtestpri
echo "PRE_TARGETDEPS += \$\$PWD/googletest/build/lib/libgtest.a" >> $gtestpri



