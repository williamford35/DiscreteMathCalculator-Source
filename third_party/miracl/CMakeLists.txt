cmake_minimum_required(VERSION 3.23)

set(MIRACL_SOURCES
    source/big.cpp
    source/complex.cpp
    source/crt.cpp
    source/ec2.cpp
    source/ecn.cpp
    source/flash.cpp
    source/floating.cpp
    source/flpoly.cpp
    source/gf2m.cpp
    source/mraes.c
    source/mralloc.c
    source/mrarth0.c
    source/mrarth1.c
    source/mrarth2.c
    source/mrarth3.c
    source/mrbits.c
    source/mrbrick.c
    source/mrbuild.c
    source/mrcore.c
    source/mrcrt.c
    source/mrcurve.c
    source/mrdouble.c
    source/mrebrick.c
    source/mrec2m.c
    source/mrecn2.c
    source/mrfast.c
    source/mrflash.c
    source/mrflsh1.c
    source/mrflsh2.c
    source/mrflsh3.c
    source/mrflsh4.c
    source/mrfpe.c
    source/mrfrnd.c
    source/mrgcd.c
    source/mrgcm.c
    source/mrgf2m.c
    source/mrio1.c
    source/mrio2.c
    source/mrjack.c
    source/mrlucas.c
    source/mrmonty.c
    source/mrmuldv.c
    source/mrpi.c
    source/mrpower.c
    source/mrprime.c
    source/mrrand.c
    source/mrround.c
    source/mrscrt.c
    source/mrsha3.c
    source/mrshs.c
    source/mrshs256.c
    source/mrshs512.c
    source/mrsmall.c
    source/mrsroot.c
    source/mrstrong.c
    source/mrxgcd.c
    source/mrzzn2.c
    source/mrzzn2b.c
    source/mrzzn3.c
    source/p1363.c
    source/poly.cpp
    source/polymod.cpp
    source/polyxy.cpp
    source/ps_big.cpp
    source/zzn.cpp
)

# MIRACL's original build compiles its C sources as C++.
set_source_files_properties(
    ${MIRACL_SOURCES}
    PROPERTIES LANGUAGE CXX
)

add_library(miracl STATIC ${MIRACL_SOURCES})

target_include_directories(miracl PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(miracl PRIVATE
    -O2
    -Wno-narrowing
)

set_target_properties(miracl PROPERTIES
    OUTPUT_NAME miracl
)

# MIRACL compatibility header hierarchy

set(MIRACL_COMPAT_INCLUDE
    "${CMAKE_CURRENT_BINARY_DIR}/compat-include"
)

file(MAKE_DIRECTORY
    "${MIRACL_COMPAT_INCLUDE}/miracl"
)

file(GLOB MIRACL_COMPAT_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

foreach(header IN LISTS MIRACL_COMPAT_HEADERS)
    get_filename_component(header_name "${header}" NAME)

    configure_file(
        "${header}"
        "${MIRACL_COMPAT_INCLUDE}/miracl/${header_name}"
        COPYONLY
    )
endforeach()

target_include_directories(miracl PUBLIC
    "${MIRACL_COMPAT_INCLUDE}"
)
