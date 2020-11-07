CFLAGS=$(sdl2-config --cflags)
LIBS=$(sdl2-config --libs)
main: main.cpp display.cpp Mesh.cpp shader.cpp texture.cpp stb_image.cpp
	g++ $(CFLAGS) main.cpp display.cpp Mesh.cpp shader.cpp texture.cpp stb_image.cpp -o main $(LIBS) -framework OpenGL -Wno-everything

# g++ $(sdl2-config --cflags) $(gdal-config --cflags) main.cpp display.cpp Mesh.cpp shader.cpp texture.cpp stb_image.cpp -o main $(sdl2-config --libs) $(gdal-config --libs) -framework OpenGL -Wno-everything -std=c++11