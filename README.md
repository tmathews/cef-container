# cef-container

## Dependencies

- [CMake](https://cmake.org/download/)
- [CEF](http://opensource.spotify.com/cefbuilds/index.html)
- On Linux: X11 development libraries (Ubuntu: install `libx11-dev`)

## Build instructions

Set the `CEF_ROOT` environment variable to wherever you downloaded the CEF
binary distribution and follow the instructions below according to your platform/compiler.

### Visual Studio 2017

```sh
mkdir build && cd build
cmake ../ -A x64
```

### Linux

```sh
mkdir build && cd build
cmake ../
```

Or use the scripts:
```sh
cd scripts
./build.sh
./run.sh
```
