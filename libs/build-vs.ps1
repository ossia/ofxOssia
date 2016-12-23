function Expand-ZIPFile($file, $destination)
{
  $shell = new-object -com shell.application
  $zip = $shell.NameSpace($file)
  foreach($item in $zip.items())
  {
    $shell.Namespace($destination).copyhere($item)
  }
}

. (Resolve-Path "$env:LOCALAPPDATA\GitHub\shell.ps1")
git clone --recursive https://github.com/OSSIA/API

$boost_url = "http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.zip"
$boost_output = "$PSScriptRoot\boost.zip"
Invoke-WebRequest -Uri $boost_url -OutFile $boost_output
Expand-ZIPFile("boost.zip", "$PSScriptRoot")

$cmake_url = "https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.zip"
$cmake_output = "$PSScriptRoot\cmake.zip"
Invoke-WebRequest -Uri $cmake_url -OutFile $cmake_output
Expand-ZIPFile("cmake.zip", "$PSScriptRoot")

mkdir build
cd build
cmake ../API -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_INSTALL_PREFIX=ossia-inst \
             -DOSSIA_TESTING=0 \
             -DOSSIA_STATIC=1 \
             -DOSSIA_NO_SONAME=1 \
             -DOSSIA_PD=0 \
             -DBOOST_ROOT=$(pwd)/boost_1_58_0 \
             -DOSSIA_NO_QT=1

cmake --build . -- /p:Configuration=Release /m
cmake --build . --target Install  -- /p:Configuration=Release /m
