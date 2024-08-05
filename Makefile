src_files=src/main.cpp src/glad/glad.c src/render/stb_image.cpp src/Window/window.cpp
SRC=src
OUT=output
OBJ=objects
LIBS= -lglfw -lGL
main: src/main.cpp
	g++ $(src_files) $(LIBS) -o $(OUT)/main
	./$(OUT)/main assets/Textures/lava_solidified.jpg

release: src/main.cpp
	g++ $(src_files) $(LIBS) -Os -o $(OUT)/main
	./$(OUT)/main assets/Textures/stone_rocks.jpg

