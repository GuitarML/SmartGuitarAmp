#!/bin/bash

build64(){
    #cmake -Bbuild -G"Visual Studio 15 2017 Win64"
    cmake -Bbuild -G "Visual Studio 16 2019" -A x64
    cmake --build build --config Release -j4
}

# exit on failure
set -e

# clean up old builds
rm -Rf build/
rm -Rf bin/*Win64*

# set up VST and ASIO paths
sed -i -e "9s/#//" CMakeLists.txt
sed -i -e "10s/#//" CMakeLists.txt
sed -i -e '16s/#//' CMakeLists.txt

# cmake new builds
build64 &
wait

# copy builds to bin
mkdir -p bin/Win64
declare -a plugins=("SmartAmp")
for plugin in "${plugins[@]}"; do
    cp -R build/${plugin}_artefacts/Release/VST3/${plugin}.vst3 bin/Win64/${plugin}.vst3
done

# reset CMakeLists.txt
#git restore CMakeLists.txt

# zip builds
VERSION=$(cut -f 2 -d '=' <<< "$(grep 'CMAKE_PROJECT_VERSION:STATIC' build/CMakeCache.txt)")
(
    cd bin
    rm -f "SmartAmp-Win64-${VERSION}.zip"
    tar -a -c -f "SmartAmp-Win64-${VERSION}.zip" Win64
)

# create installer
echo "Creating installer..."
(
    cd installers/windows
    bash build_win_installer.sh
)
