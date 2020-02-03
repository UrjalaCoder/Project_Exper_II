#ifndef TERRAIN
#define TERRAIN

#include "../PerlinNoise/PerlinNoise.h"
#include <vector>

struct Vertex3
{
	float x, y, z;
};

class Terrain
{
public:
	Terrain(float offset);
	std::vector<Vertex3> getGeometry();
};

#endif
