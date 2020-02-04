OBJS = ./src/main.cpp ./src/Renderer/Renderer.cpp ./src/Shader/Shader.cpp ./src/Terrain/Terrain.cpp ./src/PerlinNoise/PerlinNoise.cpp
LINK_OBJS = main.o Renderer.o Shader.o Terrain.o PerlinNoise.o
LINKER_OPTIONS =  -lSDL2 -lGLEW -lGLU -lGL
OBJ_NAME = exper

# This is the target that compiles our executable
all: $(OBJS)
	@echo "Building"
	g++ -c -w -std=c++14 $(OBJS) -I.
	g++ -w $(LINK_OBJS) $(LINKER_OPTIONS) -o $(OBJ_NAME)
	@echo "Cleaning build files"
	rm -f $(LINK_OBJS)
	rm -f TextureLoader.o
	mv $(OBJ_NAME) build/$(OBJ_NAME)
