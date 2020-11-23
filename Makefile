CC=g++
TARGET=quant
CFLAGS=-framework OpenGL -Wno-everything -std=c++11 $$(sdl2-config --cflags) $$(gdal-config --cflags)
LIBS=$$(sdl2-config --libs) $$(gdal-config --libs)

$(TARGET): main.o display.o Mesh.o shader.o texture.o Mouse.o stb_image.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIBS)

main.o: main.cpp display.o Mesh.o shader.o texture.o Mouse.o stb_image.o
	$(CC) $(CFLAGS) -c main.cpp

Mouse.o: Mouse.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

stb_image.o: stb_image.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

texture.o: texture.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

shader.o: shader.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

Mesh.o: Mesh.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

display.o: display.cpp
	$(CC) $(CFLAGS) -c $^ $(LIBS)

clean: 
	$(RM) $(TARGET) *.o *~
