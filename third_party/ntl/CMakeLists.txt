# NTL 11.5.1 — build from source for macOS ARM64.

set(NTL_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}")
set(NTL_BUILD "${CMAKE_CURRENT_BINARY_DIR}/ntl-build")

file(MAKE_DIRECTORY "${NTL_BUILD}/src")
set(NTL_LIBRARY "${NTL_BUILD}/src/ntl.a")

# Use GMP built automatically by CMake.
set(GMP_PREFIX "${PROJECT_BINARY_DIR}/third_party/gmp/install")
set(GMP_STATIC "${GMP_PREFIX}/lib/libgmp.a")

# NTL generates headers during configuration and compilation.
# These headers must exist before application sources are compiled.
set(NTL_INCLUDE "${NTL_BUILD}/include")

file(MAKE_DIRECTORY "${NTL_INCLUDE}")

add_custom_command(
    OUTPUT "${NTL_LIBRARY}"

    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${NTL_SOURCE}/src"
            "${NTL_BUILD}/src"

    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${NTL_SOURCE}/include"
            "${NTL_INCLUDE}"

    COMMAND "${CMAKE_COMMAND}" -E env
            "MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}"
            /bin/sh ./configure
            CXX=/usr/bin/clang++
            "CXXFLAGS=-O2 -mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}"
            NATIVE=off
            NTL_GMP_LIP=on
            NTL_THREADS=on
            NTL_THREAD_BOOST=on
            NTL_STD_CXX11=on
            GMP_PREFIX=${GMP_PREFIX}

    COMMAND /usr/bin/make
            GMP_OPT_LIB=${GMP_STATIC}
            -j4

    WORKING_DIRECTORY "${NTL_BUILD}/src"

    DEPENDS
        "${NTL_SOURCE}/src/DoConfig"
        "${NTL_SOURCE}/src/mfile"
        "${NTL_SOURCE}/src/FFT.cpp"
        "${NTL_SOURCE}/src/ZZ.cpp"
        "${GMP_STATIC}"

    COMMENT "Configuring and building NTL 11.5.1"
    VERBATIM
)

add_custom_target(ntl_build DEPENDS "${NTL_LIBRARY}")

# GMP must be installed before NTL is configured and built.
add_dependencies(ntl_build gmp_build)

add_library(ntl STATIC IMPORTED GLOBAL)

set_target_properties(ntl PROPERTIES
    IMPORTED_LOCATION "${NTL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${NTL_INCLUDE}"
)

add_dependencies(ntl ntl_build)
