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

	shader = new Shader("./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");
	textShader = new Shader("./src/shaders/vertex_text_shader.glsl", "./src/shaders/fragment_text_shader.glsl");
	// Initialize FreeType2
	initializeTextRenderer();
}

void Renderer::printError(std::string error)
{
	std::cout << "[ERROR]: " << error << std::endl;
}

int Renderer::render(std::vector<Vertex3> terrainPoints, std::vector<glm::vec3> terrainNormals, GLuint vertexArrayObject, GLuint vertexBufferObject)
{
	int featureCount = 6;
	GLfloat vertices[terrainPoints.size() * featureCount];
	// -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	// -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	// -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//
	// -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	//  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	//  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	//  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	// -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	// -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	//
	// -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	// -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	// -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	// -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	// -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	// -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	//
	//  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	//  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	//  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	//
	// -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	//  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	//  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	//  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	// -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	// -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	//
	// -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	//  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	//  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	//  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	// -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	// -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

	for(int i = 0; i < terrainPoints.size(); i++)
	{
		int vertexIndex = i * featureCount;
		vertices[vertexIndex] = (float)terrainPoints[i].x;
		vertices[vertexIndex + 1] = (float)terrainPoints[i].y;
		vertices[vertexIndex + 2] = (float)terrainPoints[i].z;

		// Normals
		vertices[vertexIndex + 3] = (float)terrainNormals[i].x;
		vertices[vertexIndex + 4] = (float)terrainNormals[i].y;
		vertices[vertexIndex + 5] = (float)terrainNormals[i].z;
	}

	/* Start with the vertex array object */

	glBindVertexArray(vertexArrayObject);

	/* Configure the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	/* Add vertex data */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Set vertex attribute data. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* RENDER */

	shader->use();
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, terrainPoints.size());

	return 0;
}

int Renderer::renderText(
	std::string text,
	GLfloat x,
	GLfloat y,
	GLfloat scale,
	glm::vec3 color
)
{
	textShader->use();
	glUniform3f(glGetUniformLocation(textShader->ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;
	for(c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0, 0.0},
			{xpos, ypos, 0.0, 1.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos, ypos + h, 0.0, 0.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0}
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}

int Renderer::initializeTextRenderer()
{
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
	{
		printError("Could not init FreeType Library");
		return -1;
	}

	FT_Face face;
	if(FT_New_Face(ft, "/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf", 0, &face))
	{
		printError("Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Load all the characters from 0-128 to the map 'Characters'.

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(GLubyte c = 0; c < 128; c++)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printError("Failed to load Glyph");
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		this->Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	/* Initialize VertexBuffer and VertexArray for text rendering.*/

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

int Renderer::start(Terrain terrain)
{

	int middleX = terrain.terrainWidth / 2;

	double movement = 0.04f;

	shader->use();

	shader->set_vec3("lightPos", glm::vec3((float)800, 100, 100));
	shader->set_vec3("lightColor", glm::vec3(0.5, 0.5, 0.5));
	shader->set_vec3("objectColor", glm::vec3(1, 1, 1));

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	GLint modelLocation = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3((float)-middleX, -40.0f, -140.0f));
	// view = glm::translate(view, glm::vec3(0, 0, -3));
	GLint viewLocation = glGetUniformLocation(shader->ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)this->windowWidth / (float)this->windowHeight, 0.1f, 800.0f);

	GLint projectionLocation = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// Triangle drawing.
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);

	glEnable(GL_DEPTH_TEST);


	// Setting text projection.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(this->windowWidth), 0.0f, static_cast<GLfloat>(this->windowHeight));
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

	// FPS counter
	unsigned int lastTime = SDL_GetTicks();
	unsigned int frameCount = 0;
	unsigned int updateCount = 0;

	float frameRate = 60.0;
	float updateRate = 60.0;

	unsigned int lastFrameDrawTick, lastUpdateTick = SDL_GetTicks();
	double frameMillis = (1.0 / frameRate) * 1000;
	double updateMillis = (1.0 / updateRate) * 1000;

	// Background color.
	glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::string frameCountText = "";
	bool wireframe = false;
	while(this->running)
	{
		while(SDL_PollEvent(&(this->currentEvent)) != 0)
		{
			if(this->currentEvent.type == SDL_QUIT)
			{
				this->running = false;
			}

			if(this->currentEvent.type == SDL_KEYDOWN) {
				if(this->currentEvent.key.keysym.sym == SDLK_w) {
					wireframe = !wireframe;
					if(wireframe) {
						glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					} else {
						glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					}
				}
			}
		}
		unsigned int now = SDL_GetTicks();

		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Framerate counter
		if(now - lastTime > 1000) {
			frameCountText = std::to_string(frameCount) + "/" + std::to_string(updateCount);
			frameCount = 0;
			updateCount = 0;
			lastTime = now;
		}

		// MAIN_UPDATE.
		if((double)(now - lastUpdateTick) > updateMillis) {
			lastUpdateTick = SDL_GetTicks();
			updateCount++;
			terrain.move(movement);
		}

		// MAIN_RENDER
		double renderTick = (double)(now - lastFrameDrawTick);
		// std::cout << renderTick << std::endl;
		if(((double)(now - lastFrameDrawTick) > frameMillis)) {
			lastFrameDrawTick = SDL_GetTicks();
			// Terrain rendering.
			terrain.calculate();
			this->render(terrain.getTerrain(), terrain.getNormals(), vertexArrayObject, vertexBufferObject);
			if(wireframe) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
			// Render Text.
			this->renderText(frameCountText, 1.0f, (float)windowHeight - 20.0f, 0.4f, glm::vec3(0.5, 0.8f, 0.2f));
			if(wireframe) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			}
			frameCount++;
			SDL_GL_SwapWindow(this->currentWindow);
		}

	}
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->currentWindow);
	this->currentWindow = nullptr;
	SDL_Quit();
	return 0;
}
