#!/bin/sh

git clone --recursive -j4 https://github.com/OSSIA/API

mkdir build
(
cd build
cmake ../API -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia -DOSSIA_TESTING=0 -DOSSIA_STATIC=0 -DOSSIA_NO_SONAME=1 -DOSSIA_PD=0
make -j$(nproc)
make install
rm -rf ../ossia/include
mv ossia/include ../ossia/include
if [[ -d "/proc" ]] ; then
  mkdir -p ../ossia/lib/linux64
  mv ossia/lib/libossia.so* ../ossia/lib/linux64/
else
  mkdir -p ../ossia/lib/osx
  mv ossia/lib/libossia.*dylib* ../ossia/lib/osx/
fi
)
rm -rf API build
