#!/bin/bash

# exit on failure
set -e

# need to run in sudo mode on Mac
if [[ "$OSTYPE" == "darwin"* ]]; then
    if [ "$EUID" -ne 0 ]; then
        echo "This script must be run in sudo mode! Exiting..."
        exit 1
    fi
fi

if [[ "$*" = *debug* ]]; then
    echo "Making DEBUG build"
    build_config="Debug"
else
    echo "Making RELEASE build"
    build_config="Release"
fi

# clean up old builds
if [[ $* = *clean* ]]; then
    echo "Cleaning previous build..."
    rm -rf build-aax/
fi

sed_cmakelist()
{
    sed_args="$1"

    if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i '' "$sed_args" CMakeLists.txt
    else
        sed -i -e "$sed_args" CMakeLists.txt
    fi
}

# set up OS-dependent variables
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Building for MAC"
    AAX_PATH=~/Developer/AAX_SDK/
    ilok_pass=$(more ~/Developer/ilok_pass)
    aax_target_dir="/Library/Application Support/Avid/Audio/Plug-Ins"
    TEAM_ID=$(more ~/Developer/mac_id)
    TARGET_DIR="Mac"

else # Windows
    echo "Building for WINDOWS"
    AAX_PATH=C:/SDKs/AAX_SDK/
    ilok_pass=$(cat /c/SDKs/ilok_pass)
    aax_target_dir="/c/Program Files/Common Files/Avid/Audio/Plug-Ins"
    TARGET_DIR="Win64"
fi

# set up build AAX
#sed_cmakelist "s~# juce_set_aax_sdk_path.*~juce_set_aax_sdk_path(${AAX_PATH})~"

# cmake new builds
if [[ "$OSTYPE" == "darwin"* ]]; then
    cmake -Bbuild-aax -GXcode -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="Developer ID Application" \
        -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM="$TEAM_ID" \
        -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE="Manual" \
        -D"CMAKE_OSX_ARCHITECTURES=x86_64" \
        -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_INJECT_BASE_ENTITLEMENTS=NO \
        -DCMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS="--timestamp" \
        -DMACOS_RELEASE=ON

    cmake --build build-aax --config $build_config --target SmartAmp_AAX | xcpretty

else # Windows
    cmake -Bbuild-aax -G"Visual Studio 16 2019" -A x64
    cmake --build build-aax --config $build_config --parallel $(nproc) --target SmartAmp_AAX
fi

# sign with PACE
aax_location=build-aax/SmartAmp_artefacts/$build_config/AAX/SmartAmp.aaxplugin
wcguid="" # Update
if [[ "$OSTYPE" == "darwin"* ]]; then
    /Applications/PACEAntiPiracy/Eden/Fusion/Current/bin/wraptool sign --verbose \
        --account keyth72 \
        --password "$ilok_pass" \
        --wcguid $wcguid \
        --dsig1-compat off \
        --signid "Developer ID Application: Keith Bloemer" \
        --in $aax_location \
        --out $aax_location

        /Applications/PACEAntiPiracy/Eden/Fusion/Current/bin/wraptool verify --verbose --in $aax_location

else # Windows
    wraptool sign --verbose \
        --account keyth72 \
        --password "$ilok_pass" \
        --wcguid $wcguid \
        --keyfile /c/SDKs/keith_aax_cert.p12 \
        --keypassword "$ilok_pass" \
        --in $aax_location \
        --out $aax_location
        
    wraptool verify --verbose --in $aax_location/Contents/x64/SmartAmp.aaxplugin
fi

# reset AAX SDK field...
#sed_cmakelist "s~juce_set_aax_sdk_path.*~# juce_set_aax_sdk_path(NONE)~"

rm -rf "$aax_target_dir/SmartAmp.aaxplugin"
cp -R "$aax_location" "$aax_target_dir/SmartAmp.aaxplugin"

if [[ "$*" = *deploy* ]]; then
    set +e

    ssh "smartguitarml@gmail.com" "rm -r ~/aax_builds/${TARGET_DIR}/SmartAmp.aaxplugin"
    scp -r "$aax_location" "smartguitarml@gmail.com:~/aax_builds/${TARGET_DIR}/"
fi
