#!/bin/sh

script_file=SmartAmp_Install_Script.iss

app_version=$(cut -f 2 -d '=' <<< "$(grep 'CMAKE_PROJECT_VERSION:STATIC' ../../build/CMakeCache.txt)")
echo "Setting app version: $app_version..."
sed -i "s/##APPVERSION##/${app_version}/g" $script_file

# build installer
echo Building...
$"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" $script_file

# reset version number
sed -i "s/${app_version}/##APPVERSION##/g" $script_file

exec="SmartAmp-Win-$app_version.exe"
direc=$PWD


echo SUCCESS
