#!/bin/bash
# This script builds a .deb package for installing the VST3, and LV2 plugins on Linux

# Set the app name and version here
app_name=SmartAmp
version=1.3


# 1. Create the package directory structure and control file

mkdir -p $app_name"/DEBIAN"

printf "Package: $app_name\n\
Version: $version\n\
Section: custom\n\
Priority: optional\n\
Architecture: all\n\
Essential: no\n\
Installed-Size: 16480128\n\
Maintainer: GuitarML\n\
Description: GuitarML Plugin Debian Package (VST3, LV2)\n" > $app_name"/DEBIAN/control"


# 2. Copy VST3 and LV2 plugins to the package directory (assumes project is already built)

mkdir -p $app_name/usr/local/lib/vst3/
echo "Copying ../../build/"$app_name"_artefacts/Release/VST3/"$app_name".vst3"
cp -r "../../build/"$app_name"_artefacts/Release/VST3/"$app_name".vst3" $app_name"/usr/local/lib/vst3/"

mkdir -p $app_name/usr/local/lib/lv2/
echo "Copying ../../build/"$app_name"_artefacts/Release/LV2/"$app_name".lv2"
cp -r "../../build/"$app_name"_artefacts/Release/LV2/"$app_name".lv2" $app_name"/usr/local/lib/lv2/"


# 3. Build the .deb package and rename

dpkg-deb --build $app_name

mv $app_name.deb $app_name-Linux-x64-$version.deb
