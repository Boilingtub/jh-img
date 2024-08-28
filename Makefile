src_files=src/main.cpp src/glad/glad.c src/render/stb_image.cpp
SRC=src
OUT=output
OBJ=objects
LIBS= -lglfw -lGL
main: src/main.cpp
	g++ -g $(src_files) $(LIBS) -o $(OUT)/main
	gdb -ex=r --batch --args ./$(OUT)/main assets/Textures/PushDocLogo.png ;

release: src/main.cpp
	g++ $(src_files) $(LIBS) -O3 -o $(OUT)/main
	./$(OUT)/main assets/Textures/stone_rocks.jpg

