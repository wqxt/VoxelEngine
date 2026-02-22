#include "VoxelMeshGenerator.h"
#include <render/Mesh.h>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <functional>


static bool isSolid(const VoxelData& data, int x, int y, int z,
	int chunkWorldX, int chunkWorldY, int chunkWorldZ,
	std::function<bool(int, int, int)> checkNeighborChunkFunc)
{

	if (x >= 0 && x < data.GetSizeX() &&
		y >= 0 && y < data.GetSizeY() &&
		z >= 0 && z < data.GetSizeZ()) {
		return data.GetType(x, y, z) != VoxelType::Air;
	}

	int worldX = chunkWorldX + x;
	int worldY = chunkWorldY + y;
	int worldZ = chunkWorldZ + z;
	return checkNeighborChunkFunc(worldX, worldY, worldZ);
}

static void setColorForType(uint8_t type, float& r, float& g, float& b)
{
	switch (type)
	{
	case VoxelType::Stone: r = 0.5f;  g = 0.5f;  b = 0.55f; break;
	case VoxelType::Dirt:  r = 0.45f; g = 0.35f; b = 0.25f; break;
	case VoxelType::Grass: r = 0.2f;  g = 0.6f;  b = 0.2f;  break;
	default:               r = 0.5f;  g = 0.5f;  b = 0.5f;  break;
	}
}

static void addQuad(std::vector<float>& positions, std::vector<float>& colors, std::vector<float>& uvs,
	std::vector<GLuint>& indices,
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float r, float g, float b)
{

	GLuint base = static_cast<GLuint>(positions.size() / 3);
	positions.push_back(x0); positions.push_back(y0); positions.push_back(z0);
	positions.push_back(x1); positions.push_back(y1); positions.push_back(z1);
	positions.push_back(x2); positions.push_back(y2); positions.push_back(z2);
	positions.push_back(x3); positions.push_back(y3); positions.push_back(z3);

	for (int i = 0; i < 4; ++i)
	{
		colors.push_back(r); colors.push_back(g); colors.push_back(b);
	}


	uvs.push_back(0.f); uvs.push_back(1.f);
	uvs.push_back(1.f); uvs.push_back(0.f);
	uvs.push_back(1.f); uvs.push_back(1.f);
	uvs.push_back(0.f); uvs.push_back(1.f);


	indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
	indices.push_back(base + 0); indices.push_back(base + 2); indices.push_back(base + 3);
}

void VoxelMeshGenerator::GenerateMesh(const VoxelData& data, Mesh& outMesh,
	int chunkWorldX, int chunkWorldY, int chunkWorldZ,
	std::function<bool(int, int, int)> checkNeighborChunkFunc)
{
	std::vector<float> positions;
	std::vector<float> colors;
	std::vector<float> uvs;
	std::vector<GLuint> indices;

	for (int z = 0; z < data.GetSizeZ(); ++z)
	{
		for (int y = 0; y < data.GetSizeY(); ++y)
		{
			for (int x = 0; x < data.GetSizeX(); ++x)
			{
				uint8_t type = data.GetType(x, y, z);

				if (type == VoxelType::Air)
				{
					continue;
				}

				float cr, cg, cb;
				setColorForType(type, cr, cg, cb);

				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				float fz = static_cast<float>(z);

				const float topShade = 1.f;
				const float bottomShade = 0.45f;
				const float sideShade = 0.75f;

				if (!isSolid(data, x + 1, y, z, chunkWorldX, chunkWorldY,chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs, indices,
						fx + 1, fy, fz,
						fx + 1, fy + 1, fz,
						fx + 1, fy + 1, fz + 1,
						fx + 1, fy, fz + 1,
						cr * sideShade, cg * sideShade, cb * sideShade);
				}
				if (!isSolid(data, x - 1, y, z, chunkWorldX, chunkWorldY, chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs, indices,
						fx, fy, fz + 1,
						fx, fy + 1, fz + 1,
						fx, fy + 1, fz,
						fx, fy, fz,
						cr * sideShade, cg * sideShade, cb * sideShade);
				}
				if (!isSolid(data, x, y + 1, z, chunkWorldX, chunkWorldY, chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs,indices,
						fx, fy + 1, fz,
						fx + 1, fy + 1, fz,
						fx + 1, fy + 1, fz + 1,
						fx, fy + 1, fz + 1,
						cr * topShade, cg * topShade, cb * topShade);
				}
				if (!isSolid(data, x, y - 1, z, chunkWorldX, chunkWorldY, chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs,indices,
						fx, fy, fz + 1,
						fx + 1, fy, fz + 1,
						fx + 1, fy, fz,
						fx, fy, fz,
						cr * bottomShade, cg * bottomShade, cb * bottomShade);
				}
				if (!isSolid(data, x, y, z + 1, chunkWorldX, chunkWorldY, chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs,indices,
						fx + 1, fy, fz + 1,
						fx + 1, fy + 1, fz + 1,
						fx, fy + 1, fz + 1,
						fx, fy, fz + 1,
						cr * sideShade, cg * sideShade, cb * sideShade);
				}
				if (!isSolid(data, x, y, z - 1, chunkWorldX, chunkWorldY, chunkWorldZ, checkNeighborChunkFunc)) {
					addQuad(positions, colors, uvs,indices,
						fx, fy, fz,
						fx + 1, fy, fz,
						fx + 1, fy + 1, fz,
						fx, fy + 1, fz,
						cr * sideShade, cg * sideShade, cb * sideShade);
				}
			}
		}
	}

	outMesh.Init(positions.data(), positions.size(),
		colors.data(), colors.size(),
		uvs.data(), uvs.size(),
		indices.data(), indices.size());
}
