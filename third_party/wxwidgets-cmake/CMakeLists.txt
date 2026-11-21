if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.23)
    project(wxWidgetsSourceBuild LANGUAGES C CXX)
endif()

include(ExternalProject)

set(WX_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../wxwidgets")

set(WX_BUILD_DIR
    "${CMAKE_BINARY_DIR}/wxwidgets-source-build")

set(WX_INSTALL_DIR
    "${CMAKE_BINARY_DIR}/wxwidgets-source-install")

ExternalProject_Add(wxwidgets_source_build
    SOURCE_DIR "${WX_SOURCE_DIR}"
    BINARY_DIR "${WX_BUILD_DIR}"

    CONFIGURE_COMMAND
        "${WX_SOURCE_DIR}/configure"
        --prefix=${WX_INSTALL_DIR}
        --with-osx_cocoa
        --with-macosx-version-min=15.0
        --enable-unicode
        --disable-shared
        --with-regex=builtin
        --with-libpng=builtin
        --with-libjpeg=builtin
        --with-libtiff=builtin
        --with-expat=builtin
        --without-libiconv
        --without-libcurl

    BUILD_COMMAND
        /usr/bin/make -j4

    INSTALL_COMMAND
        /usr/bin/make install

    BUILD_IN_SOURCE OFF
)
