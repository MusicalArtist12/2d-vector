#! /bin/sh

rm -rf build/binary/assets
mkdir build/binary/assets
cp -r src/assets build/binary/assets

cd ../build/binary; ./a.out