add_subdirectory(JUCE)

include(cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

# Link to libsamplerate
set(BUILD_TESTING OFF CACHE BOOL "Don't build libsamplerate tests!")
add_subdirectory(libsamplerate)
subproject_version(libsamplerate libsamplerate_version)
message(STATUS "VERSION for libsamplerate: ${libsamplerate_version}")

# link to RTNeural
set(RTNEURAL_XSIMD ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(RTNeural)

include(cmake/WarningFlags.cmake)
add_library(juce_plugin_modules STATIC)
add_subdirectory(json)
add_subdirectory(chowdsp_utils)

target_link_libraries(juce_plugin_modules
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_dsp
	nlohmann_json::nlohmann_json
	RTNeural
	samplerate
	chowdsp_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        warning_flags
)

target_compile_definitions(juce_plugin_modules
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JucePlugin_Manufacturer="GuitarML"
        JucePlugin_VersionString="${CMAKE_PROJECT_VERSION}"
        JucePlugin_Name="${CMAKE_PROJECT_NAME}"
        CHOWDSP_USE_LIBSAMPLERATE=1
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,COMPILE_DEFINITIONS>
)

target_include_directories(juce_plugin_modules
    PUBLIC
        RTNeural
        RTNeural/modules/xsimd/include
        libsamplerate/include
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)
