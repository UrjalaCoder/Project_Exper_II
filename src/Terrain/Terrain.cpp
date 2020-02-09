#include "Terrain.h"

const unsigned int SEED = 523;

// Get the normal vector for three points.
glm::vec3 getNormal(Vertex3 a, Vertex3 b, Vertex3 c)
{
	glm::vec3 aVector(a.x - b.x, a.y - b.y, a.z - b.z);
	glm::vec3 bVector(c.x - b.x, c.y - b.y, c.z - b.z);

	glm::vec3 result = glm::cross(aVector, bVector);

	return result;
}

Terrain::Terrain(int width, int height, double offset)
{
	// TODO: Implement initialization.
	currentOffset = offset;
	terrainWidth = width;
	terrainHeight = height;
	noiseGen = PerlinNoise(SEED);

	// Testing vertex.
	Vertex3 a = {0, 0, 0};
	Vertex3 b = {0, 1, 0};
	Vertex3 c = {1, 1, 0};

	glm::vec3 result = getNormal(a, b, c);
	std::cout << "RESULT: " << result.x << ", " << result.y << ", " << result.z << std::endl;
}

void Terrain::move(double movement)
{
	currentOffset = currentOffset + movement;
}

std::vector<Vertex3> Terrain::getTerrain()
{
	return this->terrain;
}

std::vector<glm::vec3> Terrain::getNormals()
{
	return this->normals;
}

void Terrain::calculate()
{
	double vertexYOff = 0;
	double yOff = currentOffset;
	double movement = 0.10;
	double geometryDensity = 6.0;
	double outputEnd = 100;

	int heightNoiseCount = (int)(terrainHeight / geometryDensity);
	int widthNoiseCount = (int)(terrainWidth / geometryDensity);

	std::vector<std::vector<Vertex3>> terrain(heightNoiseCount, std::vector<Vertex3>(widthNoiseCount, { 0, 0, 0 }));

	for(int y = 0; y < heightNoiseCount; y++)
	{
		double xOff = 0;
		double vertexXOff = 0;
		for(int x = 0; x < widthNoiseCount; x++)
		{
			double noise = noiseGen.noise(xOff, yOff, 0);
			double slope = 1.0 * (outputEnd) / (-1);
			double output = slope * (noise);

			Vertex3 newVertex({ vertexXOff, vertexYOff, output });
			terrain[y][x] = newVertex;

			vertexXOff += geometryDensity;
			xOff += movement;
		}
		yOff += movement;
		vertexYOff += geometryDensity;
	}

	// NOTE: 'result' is 1-dimensional.
	std::vector<Vertex3> newTerrain;
	std::vector<glm::vec3> newNormals;
	for(int y = 0; y < terrain.size() - 1; y++)
	{
		for(int x = 0; x < terrain[y].size() - 1; x++)
		{
			// Bottom left
			newTerrain.push_back(terrain[y][x]);
			// Top left
			newTerrain.push_back(terrain[y+1][x]);
			// Top right
			newTerrain.push_back(terrain[y+1][x+1]);

			// First normal.
			newNormals.push_back(getNormal(terrain[y][x], terrain[y+1][x], terrain[y+1][x+1]));

			// Bottom left
			newTerrain.push_back(terrain[y][x]);
			// Bottom right
			newTerrain.push_back(terrain[y][x+1]);
			// Top right
			newTerrain.push_back(terrain[y+1][x+1]);

			// Second normal
			newNormals.push_back(getNormal(terrain[y][x], terrain[y][x+1], terrain[y+1][x+1]));
		}
	}

	this->terrain = newTerrain;
	this->normals = newNormals;
}
