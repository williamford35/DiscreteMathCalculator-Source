# GMP 6.2.1 and GMPXX — build from source.

include(ExternalProject)

set(GMP_INSTALL_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/install")

set(GMP_LIBRARY
    "${GMP_INSTALL_DIR}/lib/libgmp.a")

set(GMPXX_LIBRARY
    "${GMP_INSTALL_DIR}/lib/libgmpxx.a")

ExternalProject_Add(gmp_build
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/build"

    CONFIGURE_COMMAND
        "${CMAKE_COMMAND}" -E env
        "MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        "CC=/usr/bin/clang"
        "CXX=/usr/bin/clang++"
        "CFLAGS=-O2 -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        "CXXFLAGS=-O2 -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        "LDFLAGS=-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        "${CMAKE_CURRENT_SOURCE_DIR}/configure"
        --prefix=${GMP_INSTALL_DIR}
        --enable-cxx
        --disable-shared
        --enable-static
        --disable-assembly


    BUILD_COMMAND
        "${CMAKE_COMMAND}" -E env
        "MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        /usr/bin/make -j4

    INSTALL_COMMAND
        "${CMAKE_COMMAND}" -E env
        "MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}"
        /usr/bin/make install

    BUILD_BYPRODUCTS
        "${GMP_LIBRARY}"
        "${GMPXX_LIBRARY}"
)

# Imported targets for linking.
file(MAKE_DIRECTORY "${GMP_INSTALL_DIR}/include")

add_library(gmp STATIC IMPORTED GLOBAL)
set_target_properties(gmp PROPERTIES
    IMPORTED_LOCATION "${GMP_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GMP_INSTALL_DIR}/include"
)
add_dependencies(gmp gmp_build)

add_library(gmpxx STATIC IMPORTED GLOBAL)
set_target_properties(gmpxx PROPERTIES
    IMPORTED_LOCATION "${GMPXX_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GMP_INSTALL_DIR}/include"
)
add_dependencies(gmpxx gmp_build)
