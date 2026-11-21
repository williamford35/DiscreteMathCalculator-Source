cmake_minimum_required(VERSION 3.23)

project(VFLib LANGUAGES CXX)

add_library(vf STATIC
    src/argedit.cc
    src/argraph.cc
    src/argloader.cc
    src/error.cc
    src/gene.cc
    src/gene_mesh.cc
    src/match.cc
    src/ull_state.cc
    src/ull_sub_state.cc
    src/vf_mono_state.cc
    src/vf_state.cc
    src/vf_sub_state.cc
    src/vf2_state.cc
    src/vf2_sub_state.cc
    src/vf2_mono_state.cc
    src/sd_state.cc
    src/sortnodes.cc
    src/xsubgraph.cc
)

target_include_directories(vf PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(vf PROPERTIES
    OUTPUT_NAME vf
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)


# VFLib compatibility header hierarchy
set(VFLIB_COMPAT_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/compat-include")

file(MAKE_DIRECTORY "${VFLIB_COMPAT_INCLUDE}/vflib")

file(GLOB VFLIB_COMPAT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

foreach(header IN LISTS VFLIB_COMPAT_HEADERS)
    get_filename_component(header_name "${header}" NAME)
    configure_file(
        "${header}"
        "${VFLIB_COMPAT_INCLUDE}/vflib/${header_name}"
        COPYONLY
    )
endforeach()

target_include_directories(vf PUBLIC
    "${VFLIB_COMPAT_INCLUDE}"
)
