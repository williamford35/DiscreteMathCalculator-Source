# Generated support application configuration.
add_executable(DMC_Alice
    "src/BlowFish.cpp"
    "src/DiffieHellmanAlice.cpp"
    "src/DiffieHellmanAliceMain.cpp"
    "src/ImageFiles.cpp"
    "src/RSAFunctions.cpp"
    "src/report.cpp"
)

set_target_properties(DMC_Alice PROPERTIES
    OUTPUT_NAME "Alice"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Demonstrations"
)

dmc_configure_support_app(DMC_Alice)
