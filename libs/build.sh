#!/bin/sh

git clone --recursive -j4 https://github.com/OSSIA/API -b wip/dataspace_2

mkdir build
(
cd build
cmake ../API -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia -DOSSIA_TESTING=0 -DOSSIA_STATIC=0
make -j8
make install
rm -rf ../ossia/include
mv ossia/include ../ossia/include
mv ossia/lib/libossia.so* ../ossia/lib/linux64/
)
rm -rf API build
