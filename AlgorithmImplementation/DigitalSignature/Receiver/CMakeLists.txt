# Digital Signature Receiver demonstration application.

add_executable(DMC_Receiver
    src/DigitalSignatureReceiver.cpp
    src/DigitalSignatureReceiverMain.cpp
    src/FileReceiveThread.cpp
    src/ImageFiles.cpp
    src/mersenne.cpp
    src/mrshs.cpp
    src/report.cpp
    src/RSAFunctions.cpp
    src/TextDisplay.cpp
)

set_target_properties(DMC_Receiver PROPERTIES
    OUTPUT_NAME "Receiver"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Demonstrations"
)

set_target_properties(DMC_Receiver PROPERTIES
    SKIP_BUILD_RPATH FALSE
    BUILD_RPATH "${PROJECT_ROOT}/usr/Frameworks"
)

target_compile_options(DMC_Receiver PRIVATE "-std=c++11")

target_compile_definitions(DMC_Receiver PRIVATE
    NDEBUG
    _FILE_OFFSET_BITS=64
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)

target_include_directories(DMC_Receiver PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${WX_INCLUDE}"
    "${WX_SETUP_INCLUDE}"
)

target_link_directories(DMC_Receiver PRIVATE
    "${WX_LIB}"
)

target_link_libraries(DMC_Receiver PRIVATE
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
    "-liconv"
    "-liconv"
)

add_dependencies(DMC_Receiver wxwidgets_source_build)
