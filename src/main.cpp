#include "./main.h"

int main(int argc, char* argv[])
{
	Terrain terrain(700, 800, 0);
	Renderer renderer(1280, 720);
	renderer.start(terrain);
	return 0;
}
