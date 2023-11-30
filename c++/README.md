# [Advent of Code 2023](https://adventofcode.com/2023) - C++

## Linux Build Instructions
This is a standard [`CMake`](https://cmake.org/)-based project. The correct puzzle answers are specified with `CTest` tests.

### Requirements
* CMake version 3.27 or higher
* A build system like Ninja or Make
* C++ compiler/standard library supporting (at least parts of) the C++23 standard

Tested with CMake 3.27.7, Ninja 1.10.1, and Clang/libc++ 17.0.4.

### Sample Build Commands
Below the `SOURCE_DIR` environment variable is the path to the [`c++/` subdirectory](./), and `BUILD_DIR` is an arbitrary, empty directory.
```bash
cmake \
    -G "Ninja" \
    -D CMAKE_BUILD_TYPE=Debug \
    -D CMAKE_CXX_COMPILER=clang++ \
    -D CMAKE_CXX_FLAGS="-stdlib=libc++" \
    -S "${SOURCE_DIR}" \
    -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}"
ctest --test-dir "${BUILD_DIR}"
```
