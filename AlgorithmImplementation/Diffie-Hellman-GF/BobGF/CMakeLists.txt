cmake_minimum_required(VERSION 3.23)

project(BobGF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0)

get_filename_component(ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
    ABSOLUTE
)

set(USR "${ROOT}/usr")
set(WX "${CMAKE_BINARY_DIR}/wxwidgets-source-install")

add_executable(BobGF
    src/BlowFish.cpp
    src/elliptic-curves/collectBigFactors.cpp
    src/ImageFiles.cpp
    src/report.cpp
    src/elliptic-curves/securedomain.cpp
    src/elliptic-curves/schoof.cpp
    src/elliptic-curves/ecdlp.cpp
    src/elliptic-curves/ecurvelib.cpp
    src/RSAFunctions.cpp
    src/DiffieHellmanBob.cpp
    src/DiffieHellmanBobMain.cpp
)

add_dependencies(BobGF wxwidgets_source_build)

target_include_directories(BobGF PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${USR}/include"
    "${WX}/include/wx-3.2"
    "${WX}/lib/wx/include/osx_cocoa-unicode-static-3.2"
)

target_compile_definitions(BobGF PRIVATE
    NDEBUG
    _FILE_OFFSET_BITS=64
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)

target_compile_options(BobGF PRIVATE -O2 -Wall)

target_link_directories(BobGF PRIVATE
    "${WX}/lib"
    "${USR}/lib"
)

set_target_properties(BobGF PROPERTIES
    SKIP_BUILD_RPATH FALSE
    BUILD_RPATH "${USR}/Frameworks"
)

target_link_libraries(BobGF PRIVATE
    miracl
    cryptopp
    ntl
    gmp
    "${WX}/lib/libwx_osx_cocoau_xrc-3.2.a"
    "${WX}/lib/libwx_osx_cocoau_qa-3.2.a"
    "${WX}/lib/libwx_baseu_net-3.2.a"
    "${WX}/lib/libwx_osx_cocoau_html-3.2.a"
    "${WX}/lib/libwx_osx_cocoau_core-3.2.a"
    "${WX}/lib/libwx_baseu_xml-3.2.a"
    "${WX}/lib/libwx_baseu-3.2.a"
    "${WX}/lib/libwxtiff-3.2.a"
    "${WX}/lib/libwxjpeg-3.2.a"
    "${WX}/lib/libwxpng-3.2.a"
    "${WX}/lib/libwxregexu-3.2.a"
    "${WX}/lib/libwxscintilla-3.2.a"
    "${WX}/lib/libwxexpat-3.2.a"
    expat
    "-L${CMAKE_OSX_SYSROOT}/usr/lib" z
    pthread
    "-framework IOKit"
    "-framework Carbon"
    "-framework Cocoa"
    "-framework QuartzCore"
    "-framework AudioToolbox"
    "-framework OpenGL"
    "-framework AVFoundation"
    "-framework CoreMedia"
    "-weak_framework AVKit"
    "-framework WebKit"
    "-framework Security"
)
