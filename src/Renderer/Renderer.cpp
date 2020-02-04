#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	// Window initialization code.
	this->windowWidth = width;
	this->windowHeight = height;

	/* Set OpenGL versions [1, 3] */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    const std::string title = "Project Exper";

    /* Try to initialize SDL2 */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        // error = true;
		std::cout << SDL_GetError() << std::endl;
        this->printError("Failed to init!");
    } else {
        /* Initializing window */
		this->currentWindow = SDL_CreateWindow(
			"Project Exper",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);
		if(this->currentWindow == nullptr)
		{
			this->printError("Could not create window.");
		} else {
            /* Initializing render context. */
            context = SDL_GL_CreateContext(currentWindow);
            if(this->context == nullptr) {
                this->printError("Could not create render context!");
            } else {
                /* Init GLEW */
                glewInit();
                // Initialize the OpenGL viewport.
                glViewport(0, 0, this->windowWidth, this->windowHeight);
            }
        }
    }

    /* Done initialization! */
	/* Finally load shaders that we need. */
	shader = new Shader("./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");
}

void Renderer::printError(std::string error)
{
	std::cout << "[ERROR]: " << error << std::endl;
}

int Renderer::render(std::vector<Vertex3> terrainPoints, GLuint vertexArrayObject, GLuint vertexBufferObject)
{
	GLfloat vertices[terrainPoints.size() * 3];
	for(int i = 0; i < terrainPoints.size(); i++)
	{
		int vertexIndex = i * 3;
		vertices[vertexIndex] = (float)terrainPoints[i].x;
		vertices[vertexIndex + 1] = (float)terrainPoints[i].y;
		vertices[vertexIndex + 2] = (float)terrainPoints[i].z;
	}

	/* Start with the vertex array object */

	glBindVertexArray(vertexArrayObject);

	/* Configure the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	/* Add vertex data */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Set vertex attribute data. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* RENDER */

	shader->use();
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, terrainPoints.size());

	return 0;
}

int Renderer::start(Terrain terrain)
{

	int middleX = terrain.terrainWidth / 2;

	double movement = 0.05f;

	shader->use();
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	GLint modelLocation = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3((float)-middleX, -30.0f, -160.0f));

	GLint viewLocation = glGetUniformLocation(shader->ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)this->windowWidth / (float)this->windowHeight, 0.1f, 400.0f);

	GLint projectionLocation = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	while(this->running)
	{
		while(SDL_PollEvent(&(this->currentEvent)) != 0)
		{
			if(this->currentEvent.type == SDL_QUIT)
			{
				this->running = false;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->render(terrain.getGeometry(), vertexArrayObject, vertexBufferObject);

		terrain.move(movement);

	    SDL_GL_SwapWindow(this->currentWindow);
	}
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->currentWindow);
	this->currentWindow = nullptr;
	SDL_Quit();
	return 0;
}
