cmake_minimum_required(VERSION 3.23)

project(wxShapeFramework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_ARCHITECTURES arm64)

set(WX_ROOT
    "${CMAKE_BINARY_DIR}/wxwidgets-source-install"
)

set(WX_INCLUDE "${WX_ROOT}/include/wx-3.2")
set(WX_SETUP_INCLUDE
    "${WX_ROOT}/lib/wx/include/osx_cocoa-unicode-static-3.2"
)

file(GLOB WXSF_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/wxxmlserializer/*.cpp"
)

add_library(wxsf STATIC ${WXSF_SOURCES})

add_dependencies(wxsf wxwidgets_source_build)

target_compile_definitions(wxsf PRIVATE
    _FILE_OFFSET_BITS=64
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)

target_include_directories(wxsf PUBLIC
    "${WX_INCLUDE}"
    "${WX_SETUP_INCLUDE}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)



set_target_properties(wxsf PROPERTIES
    OUTPUT_NAME "wxcode_macu_wxsf-3.2"
)

install(TARGETS wxsf
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/wx
    DESTINATION include
)
