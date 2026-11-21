# Generated support application configuration.
add_executable(DMC_Bob
    "src/BlowFish.cpp"
    "src/DiffieHellmanBob.cpp"
    "src/DiffieHellmanBobMain.cpp"
    "src/ImageFiles.cpp"
    "src/RSAFunctions.cpp"
    "src/report.cpp"
)

set_target_properties(DMC_Bob PROPERTIES
    OUTPUT_NAME "Bob"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Demonstrations"
)

dmc_configure_support_app(DMC_Bob)
