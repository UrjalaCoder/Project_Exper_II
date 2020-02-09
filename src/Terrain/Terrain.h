#ifndef TERRAIN
#define TERRAIN

#include "../PerlinNoise/PerlinNoise.h"

#ifndef GLM
#define GLM
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#endif

#include <vector>
#include <cmath>
#include <iostream>

struct Vertex3
{
	double x, y, z;
};

class Terrain
{
	double currentOffset;
	PerlinNoise noiseGen;
	std::vector<Vertex3> terrain;
	std::vector<glm::vec3> normals;
public:
	int terrainWidth, terrainHeight;
	Terrain(int width, int height, double offset);
	void move(double movement);
	void calculate();
	std::vector<Vertex3> getTerrain();
	std::vector<glm::vec3> getNormals();
};

#endif
