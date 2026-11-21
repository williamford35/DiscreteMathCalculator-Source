# Generated support application configuration.
add_executable(DMC_Sender
    "src/DigitalSignatureFileTransfer.cpp"
    "src/DigitalSignatureSender.cpp"
    "src/DigitalSignatureSenderMain.cpp"
    "src/FileSendThread.cpp"
    "src/ImageFiles.cpp"
    "src/RSAFunctions.cpp"
    "src/TextDisplay.cpp"
    "src/mersenne.cpp"
    "src/mrshs.cpp"
    "src/report.cpp"
)

set_target_properties(DMC_Sender PROPERTIES
    OUTPUT_NAME "Sender"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Demonstrations"
)

dmc_configure_support_app(DMC_Sender)
