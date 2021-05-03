CC=g++
TARGET=quant
CFLAGS=-framework OpenGL -Wno-everything -std=c++11 $$(sdl2-config --cflags) $$(gdal-config --cflags)
LIBS=$$(sdl2-config --libs) $$(gdal-config --libs)

$(TARGET): main.o display.o Mesh.o shader.o texture.o Mouse.o stb_image.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIBS)

main.o: main.cpp display.o Mesh.o shader.o texture.o Mouse.o stb_image.o camera.h transform.h geometry.h Makefile
	$(CC) $(CFLAGS) -c main.cpp

Mouse.o: Mouse.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

stb_image.o: stb_image.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

texture.o: texture.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

shader.o: shader.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

Mesh.o: Mesh.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

display.o: display.cpp Makefile
	$(CC) $(CFLAGS) -c $< $(LIBS)

clean: 
	$(RM) $(TARGET) *.o *~

run: $(TARGET)
	./$(TARGET)

.PHONY: run