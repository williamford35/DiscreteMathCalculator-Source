# DiscreteMathCalculator

DiscreteMathCalculator is a graphical application for discrete mathematics, written in C++ using wxWidgets.

This repository contains the application source code and bundled third-party library sources for building on macOS Apple Silicon.

## System Requirements

- macOS 15.0 or later

- Apple Silicon (ARM64)

- Xcode with the macOS SDK

- CMake 3.23 or later

## Building from Source

Open Terminal and change to the directory containing the downloaded source code.

1. Configure the Xcode project

```sh
cmake -S . -B build-release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0
```

2. Build DiscreteMathCalculator

```sh
cmake --build build-release -j4
```

The initial build may take considerable time because the bundled third-party libraries are also compiled from source.

3. Run the application

```sh
open build-release/DiscreteMathCalculator.app
```

## Bundled Third-Party Libraries

The source distribution includes:

- wxWidgets 3.2.2.1

- wxShapeFramework

- LEMON

- VFLib

- nauty

- GMP and GMPXX

- NTL

- MIRACL

- Crypto++

- OGDF and COIN

These libraries are integrated into the CMake build.

## Additional Programs

The project also contains demonstration programs and utilities for digital signatures, Diffie-Hellman key exchange, elliptic curves, quadratic sieves, and other algorithms.

The main CMake configuration includes targets for these programs.

## Build Status

The Release configuration has been successfully compiled and the application launched on macOS Apple Silicon.

A separate source checkout was successfully configured and built using Unix Makefiles on macOS Apple Silicon. The resulting application was launched and confirmed to run correctly. Building on a different computer has not yet been verified.

## License

See LICENSE and MIT-license-notice.txt for project licensing information.

Bundled third-party libraries retain their respective licenses.
