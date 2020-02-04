#include "Terrain.h"

const unsigned int SEED = 1013;

Terrain::Terrain(int width, int height, double offset)
{
	// TODO: Implement initialization.
	currentOffset = offset;
	terrainWidth = width;
	terrainHeight = height;
	noiseGen = PerlinNoise(SEED);
}

void Terrain::move(double movement)
{
	currentOffset = currentOffset + movement;
}

std::vector<Vertex3> Terrain::getGeometry()
{
	std::vector<std::vector<Vertex3>> terrain;

	double vertexYOff = 0;
	double yOff = currentOffset;
	double movement = 0.14;
	double geometryDensity = 4.0;
	double outputEnd = 80;
	while(vertexYOff < terrainHeight)
	{
		double xOff = 0;
		std::vector<Vertex3> row;
		double vertexXOff = 0;
		while(vertexXOff < terrainWidth)
		{
			double noise = noiseGen.noise(xOff, yOff, 0);

			double slope = 1.0 * (outputEnd) / (-1);
			double output = slope * (noise);

			Vertex3 newVertex({ vertexXOff, vertexYOff, output });
			row.push_back(newVertex);

			vertexXOff += geometryDensity;

			xOff += movement;
		}
		terrain.push_back(row);
		yOff += movement;
		vertexYOff += geometryDensity;
	}

	// NOTE: 'result' is 1-dimensional.
	std::vector<Vertex3> result;
	for(int y = 0; y < terrain.size() - 1; y++)
	{
		for(int x = 0; x < terrain[y].size() - 1; x++)
		{
			// Bottom left
			result.push_back(terrain[y][x]);
			// Top left
			result.push_back(terrain[y+1][x]);
			// Top right
			result.push_back(terrain[y+1][x+1]);
			// Bottom left
			result.push_back(terrain[y][x]);
			// Bottom right
			result.push_back(terrain[y][x+1]);
			// Top right
			result.push_back(terrain[y+1][x+1]);
		}
	}

	return result;
}
