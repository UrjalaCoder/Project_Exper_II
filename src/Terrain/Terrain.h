#ifndef TERRAIN
#define TERRAIN

#include "../PerlinNoise/PerlinNoise.h"
#include <vector>
#include <cmath>

struct Vertex3
{
	double x, y, z;
};

class Terrain
{
	double currentOffset;
	PerlinNoise noiseGen;
public:
	int terrainWidth, terrainHeight;
	Terrain(int width, int height, double offset);
	void move(double movement);
	std::vector<Vertex3> getGeometry();
};

#endif
