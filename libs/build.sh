#!/bin/sh

if [[ ! -d "API" ]]; then
  git clone --recursive https://github.com/OSSIA/API
fi

mkdir build
(
cd build

if [[ -d "/proc" ]]; then
  export OS_IS_LINUX=1
else
  export OS_IS_OSX=1

  wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
  tar -xzf boost_1_58_0.tar.gz
fi

cmake ../API -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia-inst -DOSSIA_TESTING=0 -DOSSIA_STATIC=1 -DOSSIA_NO_SONAME=1 -DOSSIA_PD=0 -DBOOST_ROOT=$(pwd)/boost_1_58_0
make -j8
make install
rm -rf ../ossia/include
mkdir -p ../ossia
mv ossia-inst/include ../ossia/include
if [[ "$OS_IS_LINUX" -eq "1" ]] ; then
  mkdir -p ../ossia/lib/linux64
  mv ossia-inst/lib/libossia.so* ../ossia/lib/linux64/
else
  mkdir -p ../ossia/lib/osx
  mv ossia-inst/lib/static/libossia.a ../ossia/lib/osx/
  cp -rf boost_1_58_0/boost ../ossia/include
fi
)
# rm -rf API build
