#!/bin/sh

git clone --recursive -j4 https://github.com/OSSIA/API

mkdir build
(
cd build
cmake ../API -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia-inst -DOSSIA_TESTING=0 -DOSSIA_STATIC=1 -DOSSIA_NO_SONAME=1 -DOSSIA_PD=0
make -j$(nproc)
make install
rm -rf ../ossia/include
mkdir -p ../ossia
mv ossia-inst/include ../ossia/include
if [[ -d "/proc" ]] ; then
  mkdir -p ../ossia/lib/linux64
  mv ossia-inst/lib/libossia.so* ../ossia/lib/linux64/
else
  mkdir -p ../ossia/lib/osx
  mv ossia-inst/lib/static/libossia.a ../ossia/lib/osx/
fi
)
# rm -rf API build
