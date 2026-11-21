cmake_minimum_required(VERSION 3.23)

project(DiscreteMathCalculator LANGUAGES C CXX)

set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(WX_ROOT
    "${CMAKE_BINARY_DIR}/wxwidgets-source-install"
)

set(WX_INCLUDE
    "${WX_ROOT}/include/wx-3.2"
)

set(WX_SETUP_INCLUDE
    "${WX_ROOT}/lib/wx/include/osx_cocoa-unicode-static-3.2"
)

set(WX_LIB
    "${WX_ROOT}/lib"
)

file(GLOB_RECURSE DMC_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_ROOT}/src/*.cpp"
)

list(FILTER DMC_SOURCES EXCLUDE REGEX "/\\._[^/]*\\.cpp$")
list(FILTER DMC_SOURCES EXCLUDE REGEX "/src/elliptic-curves/generate_verifyable_curve\\.cpp$")
file(GLOB DMC_ABOUT_FILES CONFIGURE_DEPENDS
    "${PROJECT_ROOT}/usr/About-DiscreteMathCalculator/*.txt"
)
set_source_files_properties(${DMC_ABOUT_FILES} PROPERTIES
    MACOSX_PACKAGE_LOCATION "usr/About-DiscreteMathCalculator"
)

set(DMC_ICON "${PROJECT_ROOT}/DiscreteMathCalculatorProject.icns")

set_source_files_properties("${DMC_ICON}" PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

add_subdirectory(third_party/wxSF)

# Build LEMON from source.
set(LEMON_ENABLE_GLPK NO CACHE STRING "" FORCE)
set(LEMON_ENABLE_ILOG NO CACHE STRING "" FORCE)
set(LEMON_ENABLE_COIN NO CACHE STRING "" FORCE)
set(LEMON_ENABLE_SOPLEX NO CACHE STRING "" FORCE)

add_subdirectory(third_party/lemon)

# Build VFLib from source.
add_subdirectory(third_party/vflib)

# Build nauty from source.
add_subdirectory(third_party/nauty)
add_subdirectory(third_party/gmp)
add_subdirectory(third_party/ntl)
add_subdirectory(third_party/miracl)
add_subdirectory(third_party/cryptopp-cmake)
add_subdirectory(third_party/ogdf-cmake)

# Build wxWidgets 3.2.2.1 from bundled source.
add_subdirectory(third_party/wxwidgets-cmake)



add_executable(DiscreteMathCalculator MACOSX_BUNDLE
    ${DMC_SOURCES}
    ${DMC_ABOUT_FILES}
    ${DMC_ICON}
)

# Build and install wxWidgets before compiling DiscreteMathCalculator.
add_dependencies(DiscreteMathCalculator wxwidgets_source_build)

target_compile_options(DiscreteMathCalculator PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-O0>
)

target_include_directories(DiscreteMathCalculator PRIVATE
    "${PROJECT_ROOT}"
    "${PROJECT_ROOT}/include"
    "${WX_INCLUDE}"
    "${WX_SETUP_INCLUDE}"
)

target_compile_definitions(DiscreteMathCalculator PRIVATE
    _FILE_OFFSET_BITS=64
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)

target_compile_options(DiscreteMathCalculator PRIVATE
    "-std=c++11"
)

target_link_directories(DiscreteMathCalculator PRIVATE
    "${WX_LIB}"
)

target_include_directories(DiscreteMathCalculator PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/third_party/lemon"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/lemon"
)

target_link_libraries(DiscreteMathCalculator PRIVATE

    wxsf

    OGDF
        lemon
    vf
    nauty

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
    pthread
)

set_target_properties(DiscreteMathCalculator PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "DiscreteMathCalculator"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.discretemathcalculator.DiscreteMathCalculator"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_ICON_FILE "DiscreteMathCalculatorProject.icns"
)
# Package runtime resource directories before Xcode code signing.

set(DMC_RUNTIME_DIRECTORIES
    "usr/resource"
    "usr/images"
    "usr/DigitalSignatureDocs"
    "usr/help"
    "examples"
)

foreach(DMC_DIR IN LISTS DMC_RUNTIME_DIRECTORIES)

    file(GLOB_RECURSE DMC_RESOURCE_FILES
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES FALSE
        "${PROJECT_ROOT}/${DMC_DIR}/*"
    )

    foreach(DMC_FILE IN LISTS DMC_RESOURCE_FILES)

        # DocumentServer and DocumentRequester are staged separately.
        get_filename_component(DMC_RESOURCE_NAME
            "${DMC_FILE}" NAME)

        # Exclude obsolete backups and temporary files.
        if(DMC_RESOURCE_NAME MATCHES
           "(\\.backup-|\\.before-library-fix|\\.DS_Store$|\\.cbTemp$)")
            continue()
        endif()

        # DocumentServer and DocumentRequester are staged separately.
        if(DMC_DIR STREQUAL "usr/resource" AND
           DMC_RESOURCE_NAME MATCHES "^(DocumentServer|DocumentRequester)$")
            continue()
        endif()

        file(RELATIVE_PATH DMC_RELATIVE_PATH
            "${PROJECT_ROOT}/${DMC_DIR}"
            "${DMC_FILE}"
        )

        get_filename_component(DMC_SUBDIR
            "${DMC_RELATIVE_PATH}"
            DIRECTORY
        )

    get_filename_component(DMC_TOP_DIR
        "${DMC_DIR}"
        NAME
    )


    # Install supporting executables directly in Contents/Resources.
    # Preserve existing locations for other resource files.

    set(DMC_PACKAGE_LOCATION
        "Resources/${DMC_TOP_DIR}/${DMC_SUBDIR}")

    if(DMC_DIR STREQUAL "usr/resource" AND
       DMC_RESOURCE_NAME MATCHES
       "^$")
        set(DMC_PACKAGE_LOCATION "Resources")
    endif()

    set_source_files_properties("${DMC_FILE}" PROPERTIES
        HEADER_FILE_ONLY TRUE
        MACOSX_PACKAGE_LOCATION "${DMC_PACKAGE_LOCATION}"
    )
    endforeach()

    target_sources(DiscreteMathCalculator PRIVATE
        ${DMC_RESOURCE_FILES}
    )

endforeach()

# Locate bundled dynamic libraries at runtime.
set_target_properties(DiscreteMathCalculator PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    BUILD_RPATH_USE_ORIGIN TRUE
    BUILD_RPATH "@executable_path/../Frameworks"
    INSTALL_RPATH "@executable_path/../Frameworks"
)

# Locate bundled dynamic libraries at runtime.
set_target_properties(DiscreteMathCalculator PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    BUILD_RPATH_USE_ORIGIN TRUE
    BUILD_RPATH "@executable_path/../Frameworks"
    INSTALL_RPATH "@executable_path/../Frameworks"
)

# Supporting Digital Signature Receiver application.
add_subdirectory(AlgorithmImplementation/DigitalSignature/Receiver)


# Stage the newly built Receiver into the development app bundle.
# Run this target only after building DiscreteMathCalculator.
add_custom_target(DMC_StageReceiver
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_Receiver>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/Receiver"
    DEPENDS DMC_Receiver
    COMMENT "Staging newly built Receiver into DiscreteMathCalculator.app"
    VERBATIM
)


# Shared configuration for support applications.
include("${PROJECT_ROOT}/AlgorithmImplementation/SupportAppCommon.cmake")

# Additional support applications.
add_subdirectory(AlgorithmImplementation/Diffie-Hellman/Alice)
add_subdirectory(AlgorithmImplementation/Diffie-Hellman/Bob)
add_subdirectory(AlgorithmImplementation/DigitalSignature/Sender)

# Stage the latest Digital Signature Sender executable.
add_custom_target(DMC_StageSender
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_Sender>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/Sender"
    DEPENDS DMC_Sender
    COMMENT "Staging newly built Sender into DiscreteMathCalculator.app"
    VERBATIM
)



# GF and document support applications.
add_subdirectory(AlgorithmImplementation/Diffie-Hellman-GF/AliceGF)
add_subdirectory(AlgorithmImplementation/Diffie-Hellman-GF/BobGF)
add_subdirectory(AlgorithmImplementation/DigitalSignature-GF/DocumentRequester)
add_subdirectory(AlgorithmImplementation/DigitalSignature-GF/DocumentServer)

# Stage the newly built DocumentServer and DocumentRequester
# into the development DiscreteMathCalculator application bundle.

add_custom_target(DMC_StageDocumentApps
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DocumentServer>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/DocumentServer"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DocumentRequester>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/DocumentRequester"

    DEPENDS DocumentServer DocumentRequester
    COMMENT "Staging DocumentServer and DocumentRequester"
    VERBATIM
)

# Automatically stage both document applications when
# building the main DiscreteMathCalculator application.

# Stage freshly compiled Diffie-Hellman demonstration executables.
add_custom_target(DMC_StageDiffieHellman
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_Alice>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/Alice"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_Bob>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/Bob"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:AliceGF>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/AliceGF"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:BobGF>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/BobGF"

    DEPENDS DMC_Alice DMC_Bob AliceGF BobGF

    COMMENT "Staging freshly built Diffie-Hellman executables"
    VERBATIM
)

add_dependencies(DiscreteMathCalculator DMC_StageDiffieHellman)


# Build and stage the quadratic-sieve utility.
add_subdirectory(AlgorithmImplementation/quadratic-sieve)

add_custom_target(DMC_StageQS
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:qs>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/qs"

    DEPENDS qs
    COMMENT "Staging newly built quadratic-sieve executable"
    VERBATIM
)

add_dependencies(DiscreteMathCalculator DMC_StageQS)


# Build elliptic-curve digital-signature support utilities.
set(DMC_EC_SUPPORT_DIR
    "${PROJECT_ROOT}/AlgorithmImplementation/DigitalSignature-GF/support-apps"
)

foreach(app IN ITEMS buildCurve ecsgen ecsign ecsver)
    add_executable(DMC_${app}
        "${DMC_EC_SUPPORT_DIR}/${app}.cpp"
    )

    set_target_properties(DMC_${app} PROPERTIES
        OUTPUT_NAME "${app}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ECUtilities"
    )

    target_include_directories(DMC_${app} PRIVATE
        "${DMC_EC_SUPPORT_DIR}"
    )

    target_link_libraries(DMC_${app} PRIVATE
        miracl
    )
endforeach()

add_custom_target(DMC_StageECUtilities
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_buildCurve>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/buildCurve"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_ecsgen>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/ecsgen"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_ecsign>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/ecsign"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_ecsver>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/ecsver"

    DEPENDS
        DMC_buildCurve
        DMC_ecsgen
        DMC_ecsign
        DMC_ecsver

    COMMENT "Staging elliptic-curve support utilities"
    VERBATIM
)

add_dependencies(DiscreteMathCalculator DMC_StageECUtilities)


# Build the Pollard-rho discrete-logarithm index utility.
set(DMC_INDEX_DIR
    "${PROJECT_ROOT}/AlgorithmImplementation/index"
)

add_executable(DMC_Index
    "${DMC_INDEX_DIR}/index.cpp"
    "${DMC_INDEX_DIR}/genprime.cpp"
    "${DMC_INDEX_DIR}/mersenne.cpp"
)

set_target_properties(DMC_Index PROPERTIES
    OUTPUT_NAME "index"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/IndexUtility"
)

target_include_directories(DMC_Index PRIVATE
    "${DMC_INDEX_DIR}"
)

target_link_libraries(DMC_Index PRIVATE
    miracl
)

add_custom_target(DMC_StageIndex
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_Index>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/index"

    DEPENDS DMC_Index
    COMMENT "Staging newly built index utility"
    VERBATIM
)

add_dependencies(DiscreteMathCalculator DMC_StageIndex)


# Build elliptic-curve-setup from source.
set(DMC_EC_SETUP_DIR
    "${PROJECT_ROOT}/AlgorithmImplementation/Diffie-Hellman-GF/elliptic-curve-setup"
)

add_executable(DMC_EllipticCurveSetup
    "${DMC_EC_SETUP_DIR}/main.cpp"
    "${DMC_EC_SETUP_DIR}/src/collectBigFactors.cpp"
    "${DMC_EC_SETUP_DIR}/src/schoof.cpp"
    "${DMC_EC_SETUP_DIR}/src/ellipticCurveFunctions.cpp"
)

add_dependencies(DMC_EllipticCurveSetup wxwidgets_source_build)

set_target_properties(DMC_EllipticCurveSetup PROPERTIES
    OUTPUT_NAME "elliptic-curve-setup"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/EllipticCurveSetup"
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

target_include_directories(DMC_EllipticCurveSetup PRIVATE
    "${DMC_EC_SETUP_DIR}"
    "${DMC_EC_SETUP_DIR}/include"
    "${WX_INCLUDE}"
    "${WX_SETUP_INCLUDE}"
)

target_compile_definitions(DMC_EllipticCurveSetup PRIVATE
    NDEBUG
    _FILE_OFFSET_BITS=64
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)

target_link_libraries(DMC_EllipticCurveSetup PRIVATE
    miracl
    ntl
    gmpxx
    gmp

    "${WX_LIB}/libwx_osx_cocoau_xrc-3.2.a"
    "${WX_LIB}/libwx_osx_cocoau_qa-3.2.a"
    "${WX_LIB}/libwx_baseu_net-3.2.a"
    "${WX_LIB}/libwx_osx_cocoau_html-3.2.a"
    "${WX_LIB}/libwx_osx_cocoau_core-3.2.a"
    "${WX_LIB}/libwx_baseu_xml-3.2.a"
    "${WX_LIB}/libwx_baseu-3.2.a"
    "-framework IOKit"
    "-framework Carbon"
    "-framework Cocoa"
)

add_custom_target(DMC_StageEllipticCurveSetup
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources"

    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:DMC_EllipticCurveSetup>"
        "$<TARGET_BUNDLE_CONTENT_DIR:DiscreteMathCalculator>/Resources/elliptic-curve-setup"

    DEPENDS DMC_EllipticCurveSetup
    COMMENT "Staging elliptic-curve-setup"
    VERBATIM
)

add_dependencies(DiscreteMathCalculator DMC_StageEllipticCurveSetup)

add_dependencies(DiscreteMathCalculator DMC_StageDocumentApps)



# Ensure supporting executables are staged when building
# the main DiscreteMathCalculator application.
add_dependencies(DiscreteMathCalculator
    DMC_StageReceiver
    DMC_StageSender
)
