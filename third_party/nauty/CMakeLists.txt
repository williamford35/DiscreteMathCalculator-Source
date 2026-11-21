# Build nauty 2.7r3 from source.

set(NAUTY_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NAUTY_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Generate platform-specific headers using nauty's configure script.
execute_process(
    COMMAND
        "${CMAKE_COMMAND}" -E env
        "SDKROOT=${CMAKE_OSX_SYSROOT}"
        "CC=${CMAKE_C_COMPILER}"
        "CFLAGS=-arch arm64"
        "${NAUTY_SOURCE_DIR}/configure"
    WORKING_DIRECTORY "${NAUTY_BINARY_DIR}"
    RESULT_VARIABLE NAUTY_CONFIGURE_RESULT
)

if(NOT NAUTY_CONFIGURE_RESULT EQUAL 0)
    message(FATAL_ERROR "nauty configure failed")
endif()

add_library(nauty STATIC
    nauty.c
    nautil.c
    nausparse.c
    naugraph.c
    schreier.c
    naurng.c
    traces.c
    gtools.c
    naututil.c
    nautinv.c
    gutil1.c
    gutil2.c
    gtnauty.c
    naugroup.c
    nautycliquer.c
)

target_include_directories(nauty PUBLIC
    "${NAUTY_BINARY_DIR}"
    "${NAUTY_SOURCE_DIR}"
)

set_target_properties(nauty PROPERTIES
    OUTPUT_NAME nauty
)
