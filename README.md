## Ion

Ion is a 3D viewer for spatial data. It in early stage and a lot of things are missing. I am using it mostly for learning C++ and computer graphics.

# Setup

Project uses Vcpkg to manage dependencies and CMake to build it. Please install CMake 3.20 or higher and Vcpkg. Currently only MacOS is supported. Other platforms have not be tested.

After installing CMake and Vcpkg clone repo

```
git clone git@github.com:anivanovic/ion.git
cd ion
```

and configure project
```
mkdir build && cd build
vcpkg integrate remove
cmake -DCMAKE_TOOLCHAIN_FILE=<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake ..
```

If you are using VS Code, make sure to install CMake Tools plugin. To configure project correctly you need to pass `-DCMAKE_TOOLCHAIN_FILE=<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake` option to CMake when invoked in VS Code. Follow this [tutorial](https://www.40tude.fr/how-to-use-vcpkg-with-vscode-and-cmake/) which explains it nicely. You just need to add `"cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "<VCPKG_ROOT>scripts/buildsystems/vcpkg.cmake"
    }` line to settings.json file