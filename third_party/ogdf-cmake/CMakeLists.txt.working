# Build OGDF and COIN from the included source using Unix Makefiles.

include(ExternalProject)

set(OGDF_SOURCE_DIR
    "${CMAKE_SOURCE_DIR}/third_party/ogdf"
)

set(OGDF_BUILD_DIR
    "${CMAKE_BINARY_DIR}/ogdf-source-build"
)

set(OGDF_LIBRARY "${OGDF_BUILD_DIR}/libOGDF.a")
set(COIN_LIBRARY "${OGDF_BUILD_DIR}/libCOIN.a")

file(MAKE_DIRECTORY "${OGDF_BUILD_DIR}/include")

ExternalProject_Add(ogdf_source_build
    SOURCE_DIR "${OGDF_SOURCE_DIR}"
    BINARY_DIR "${OGDF_BUILD_DIR}"

    CMAKE_GENERATOR "Unix Makefiles"

    CMAKE_ARGS
        -DCMAKE_CXX_COMPILER=/usr/bin/clang++
        -DCMAKE_OSX_ARCHITECTURES=arm64
        -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DCOIN_SOLVER=CLP

    BUILD_COMMAND
        ${CMAKE_COMMAND} --build <BINARY_DIR>
        --target OGDF --parallel 4

    INSTALL_COMMAND ""

    BUILD_BYPRODUCTS
        "${OGDF_LIBRARY}"
        "${COIN_LIBRARY}"
)

add_library(COIN STATIC IMPORTED GLOBAL)

set_target_properties(COIN PROPERTIES
    IMPORTED_LOCATION "${COIN_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES
        "${OGDF_SOURCE_DIR}/include/coin"
)

add_dependencies(COIN ogdf_source_build)

add_library(OGDF STATIC IMPORTED GLOBAL)

set_target_properties(OGDF PROPERTIES
    IMPORTED_LOCATION "${OGDF_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES
        "${OGDF_SOURCE_DIR}/include;${OGDF_BUILD_DIR}/include"
    INTERFACE_LINK_LIBRARIES COIN
)

add_dependencies(OGDF ogdf_source_build)
