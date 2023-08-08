#! /bin/sh

cd ..
rm -rf build/binary/bin
mkdir build/binary/bin 
cp -r src/bin build/binary/

cd build 

make -j8