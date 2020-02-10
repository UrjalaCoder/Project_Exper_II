#ifndef SHADER
#define SHADER

#ifndef GL
#define GL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#endif


/* Include matrix library */
#ifndef GLM
#define GLM
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#endif


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
public:
	GLuint ID;
	Shader(const GLchar *vertex_path, const GLchar *fragment_path);
	void use();

	void set_bool(const std::string &name, bool value) const;
	void set_int(const std::string &name, int value) const;
	void set_float(const std::string &name, float value) const;
	void set_vec3(const std::string &name, glm::vec3 value) const;
};

#endif
