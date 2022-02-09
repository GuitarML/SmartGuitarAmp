add_library(warning_flags INTERFACE)

if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC"))
    target_compile_options(warning_flags INTERFACE
        /W4     # base warning level
        #/wd4458 # declaration hides class member (from Foley's GUI Magic)
        /wd4505 # since VS2019 doesn't handle [[ maybe_unused ]] for static functions (RTNeural::debug_print)
        /wd4244 # for XSIMD
    )
elseif((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    target_compile_options(warning_flags INTERFACE
        -Wall -Wshadow-all -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized
        -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion
        -Wconditional-uninitialized -Woverloaded-virtual -Wreorder
        -Wconstant-conversion -Wsign-conversion -Wunused-private-field
        -Wbool-conversion -Wno-extra-semi -Wunreachable-code
        -Wzero-as-null-pointer-constant -Wcast-align
        -Wno-inconsistent-missing-destructor-override -Wshift-sign-overflow
        -Wnullable-to-nonnull-conversion -Wno-missing-field-initializers
        -Wno-ignored-qualifiers -Wpedantic -Wno-pessimizing-move
        # These lines suppress some custom warnings.
        # Comment them out to be more strict.
        -Wno-shadow-field-in-constructor
        # Supress warnings from xsimd
        -Wno-cast-align -Wno-shadow -Wno-implicit-int-conversion
        -Wno-zero-as-null-pointer-constant -Wno-sign-conversion
        # Needed for ARM processor, OSX versions below 10.14
        -fno-aligned-allocation
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(warning_flags INTERFACE
        -Wall -Wextra -Wstrict-aliasing -Wuninitialized -Wunused-parameter
        -Wsign-compare -Woverloaded-virtual -Wreorder -Wunreachable-code
        -Wzero-as-null-pointer-constant -Wcast-align -Wno-implicit-fallthrough
        -Wno-maybe-uninitialized -Wno-missing-field-initializers -Wno-pedantic
        -Wno-ignored-qualifiers -Wno-unused-function -Wno-pessimizing-move
        # From LV2 Wrapper
        -Wno-parentheses -Wno-deprecated-declarations -Wno-redundant-decls
        # For XSIMD
        -Wno-zero-as-null-pointer-constant
        # These lines suppress some custom warnings.
        # Comment them out to be more strict.
        -Wno-redundant-move
    )

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0.0")
        target_compile_options(warning_flags INTERFACE "-Wno-strict-overflow")
    endif()
endif()
