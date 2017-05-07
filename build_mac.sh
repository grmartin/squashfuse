#!/usr/bin/env bash

LZ4=$(dirname $(dirname `brew list lz4 | grep 'liblz4.a'`))

./autogen.sh
./configure --with-xz=/usr/lib/ --prefix="$HOME/unix/" --with-lz4="$LZ4" --disable-demo
make && make install && make clean
