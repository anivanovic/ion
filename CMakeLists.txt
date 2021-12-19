cmake_minimum_required(VERSION 3.20)
project(quant VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
# hacky way to instruct cmake to put exe in root project folder
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

include(CTest)
enable_testing()

find_package(GDAL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM /usr/local/lib /usr/local/include)
include_directories(${GDAL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

add_subdirectory(src/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
