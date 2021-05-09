cmake_minimum_required(VERSION 3.14)
project(quant VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB HEADERS *.h)

add_library(quant STATIC display.cpp Mesh.cpp Mouse.cpp shader.cpp stb_image.cpp texture.cpp ${HEADERS})
add_executable(ion main.cpp)
target_link_libraries(ion quant ${GDAL_LIBRARIES} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})