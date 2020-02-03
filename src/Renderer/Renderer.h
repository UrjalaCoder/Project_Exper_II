#ifndef RENDERER
#define RENDERER

/* Include SDL, GLEW and GLM */
#include <GL/glew.h>
#include <SDL2/SDL.h>

/* Include matrix library */
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

/* Include the Shader class */
#include "../Shader/Shader.h"

#include <string>
#include <iostream>
#include <cstdlib>

class Renderer
{
private:
	Shader *shader;
	SDL_Window *currentWindow;
	SDL_Event currentEvent;
	SDL_GLContext context;
	int windowWidth, windowHeight;
	bool running = true;
	void printError(std::string error);
	int render();
public:
	Renderer(int width, int height);
	int start();
};

#endif
