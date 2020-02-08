#ifndef RENDERER
#define RENDERER

/* Include SDL, GLEW and GLM */
#include <GL/glew.h>
#include <SDL2/SDL.h>

/* Include Freetype2 */
#include <ft2build.h>
#include FT_FREETYPE_H


/* Include matrix library */
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"

/* Include the Shader class */
#include "../Shader/Shader.h"
#include "../Terrain/Terrain.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <map>

struct Character {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Renderer
{
private:
	Shader *shader;
	Shader *textShader;
	SDL_Window *currentWindow;
	SDL_Event currentEvent;
	SDL_GLContext context;
	int windowWidth, windowHeight;
	GLuint textVAO;
	GLuint textVBO;
	bool running = true;
	void printError(std::string error);
	std::map<GLchar, Character> Characters;
	int render(std::vector<Vertex3> terrainPoints, GLuint vertexArrayObject, GLuint vertexBufferObject);
	int initializeTextRenderer();
	int renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
public:
	Renderer(int width, int height);
	int start(Terrain terrain);
};

#endif
