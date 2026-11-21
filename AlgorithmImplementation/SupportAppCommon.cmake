# Shared build settings for DiscreteMathCalculator support applications.
# Included from the root CMake project.

function(dmc_configure_support_app target)
    target_compile_options(${target} PRIVATE
        -std=c++11
        -O2
    )

    target_compile_definitions(${target} PRIVATE
        NDEBUG
        _FILE_OFFSET_BITS=64
        __WXMAC__
        __WXOSX__
        __WXOSX_COCOA__
    )

    target_include_directories(${target} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${WX_INCLUDE}"
        "${WX_SETUP_INCLUDE}"
    )

    # Use libraries built from the bundled source distribution.
    target_link_libraries(${target} PRIVATE
        ntl
        gmpxx
        gmp
        miracl
        cryptopp

        "${WX_LIB}/libwx_osx_cocoau_xrc-3.2.a"
        "${WX_LIB}/libwx_osx_cocoau_qa-3.2.a"
        "${WX_LIB}/libwx_baseu_net-3.2.a"
        "${WX_LIB}/libwx_osx_cocoau_html-3.2.a"
        "${WX_LIB}/libwx_osx_cocoau_core-3.2.a"
        "${WX_LIB}/libwx_baseu_xml-3.2.a"
        "${WX_LIB}/libwx_baseu-3.2.a"
        "${WX_LIB}/libwxtiff-3.2.a"
        "${WX_LIB}/libwxjpeg-3.2.a"
        "${WX_LIB}/libwxpng-3.2.a"
        "${WX_LIB}/libwxregexu-3.2.a"
        "${WX_LIB}/libwxscintilla-3.2.a"
        "${WX_LIB}/libwxexpat-3.2.a"

        "-framework IOKit"
        "-framework Carbon"
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework AudioToolbox"
        "-framework System"
        "-framework OpenGL"
        "-framework AVFoundation"
        "-framework CoreMedia"
        "-weak_framework AVKit"
        "-framework WebKit"
        "-framework Security"
        "-lexpat"
        "-lz"
    )

    add_dependencies(${target} wxwidgets_source_build)

endfunction()
