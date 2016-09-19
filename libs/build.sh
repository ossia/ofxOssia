#!/bin/sh

git clone https://github.com/OSSIA/API
(cd API ; git submodule update --init --recursive)
mkdir build
(
cd build
cmake ../API -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia -DOSSIA_TESTING=0 -DOSSIA_STATIC=0
make all_unity -j8
make install
rm -rf ../ossia/include
mv ossia/include ../ossia/include
mv ossia/lib/libossia.so* ../ossia/lib/linux64/
)
rm -rf API build
