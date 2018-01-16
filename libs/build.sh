#!/bin/bash -eux
 export PATH=$PATH:/usr/local/bin
if [[ ! -d "libossia" ]]; then
  git clone https://github.com/OSSIA/libossia
else
  (
  cd libossia
  git pull
  )
fi

mkdir -p build
(
cd build
export OS_IS_LINUX=0
export OS_IS_OSX=0
if [[ -d "/proc" ]]; then
  export OS_IS_LINUX=1
else
  export OS_IS_OSX=1

  # Set-up OS X dependencies
  export HOMEBREW_BIN=$(command -v brew)
  if [[ "$HOMEBREW_BIN" == "" ]]; then
    echo "Homebrew is not installed."
    echo "Please install it by running the following command:"
    echo '    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"'
    exit 1
  fi

  export WGET_BIN=$(command -v wget)
  if [[ "$WGET_BIN" == "" ]]; then
    brew install wget
  fi

  export CMAKE_BIN=$(command -v cmake)
  if [[ "$CMAKE_BIN" == "" ]]; then
    brew install cmake
  fi

  if [[ ! -d "boost_1_58_0" ]]; then
    wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
    tar -xzf boost_1_58_0.tar.gz
  fi

  # Changing OFX config :
  SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
  OFX_MAKE_CONFIG=$(find $SCRIPT_DIR/../../../.. -name config.osx.default.mk)
  OFX_XCODE_CONFIG=$(find $SCRIPT_DIR/../../../.. -name CoreOF.xcconfig)
  OFX_BOOST_FOLDER="$SCRIPT_DIR/../../../../libs/boost/include/boost"

  if [[ "$OFX_MAKE_CONFIG" != "" && "$OFX_XCODE_CONFIG" != "" ]]; then
	sed -i mk 's/c++11/c++14/' "$OFX_MAKE_CONFIG" "$OFX_XCODE_CONFIG"
	sed -i mk 's/10.7/10.10/' "$OFX_MAKE_CONFIG" "$OFX_XCODE_CONFIG"
  fi

fi

cmake ../libossia -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=ossia-inst -DOSSIA_PYTHON=0 -DOSSIA_NO_QT=1 -DOSSIA_TESTING=0 -DOSSIA_STATIC=1 -DOSSIA_NO_SONAME=1 -DOSSIA_PD=0 -DBOOST_ROOT=$(pwd)/boost_1_58_0 -DOSSIA_NO_QT=1 -DOSSIA_PROTOCOL_MIDI=Off
make -j8
make install
rm -rf ../ossia/include
mkdir -p ../ossia
mv ossia-inst/include ../ossia/include
if [[ "$OS_IS_LINUX" -eq "1" ]] ; then
  mkdir -p ../ossia/lib/linux64
  mv ossia-inst/lib/static/libossia.a ../ossia/lib/linux64/
else
  mkdir -p ../ossia/lib/osx
  mv ossia-inst/lib/static/libossia.a ../ossia/lib/osx/
  if [ -d "$OFX_BOOST_FOLDER/" ] ; then
    cp -rf boost_1_58_0/boost/* "$OFX_BOOST_FOLDER/"
  fi
  rm -rf boost_1_58_0
  rm -rf "$OFX_BOOST_FOLDER/asio.hpp"
  rm -rf "$OFX_BOOST_FOLDER/asio"
fi
)
rm -rf libossia build
