#include "VoxelMeshGenerator.h"
#include <render/Mesh.h>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

static bool isSolid(const VoxelData& data, int x, int y, int z)
{
	if (x < 0 || x >= data.GetSizeX()) return false;
	if (y < 0 || y >= data.GetSizeY()) return false;
	if (z < 0 || z >= data.GetSizeZ()) return false;
	return data.GetType(x, y, z) != VoxelType::Air;
}

static void addQuad(std::vector<float>& positions, std::vector<float>& colors,
	std::vector<GLuint>& indices,
	float x0, float y0, float z0, float x1, float y1, float z1,
	float x2, float y2, float z2, float x3, float y3, float z3,
	float r, float g, float b)
{
	GLuint base = static_cast<GLuint>(positions.size() / 3);
	positions.push_back(x0); positions.push_back(y0); positions.push_back(z0);
	positions.push_back(x1); positions.push_back(y1); positions.push_back(z1);
	positions.push_back(x2); positions.push_back(y2); positions.push_back(z2);
	positions.push_back(x3); positions.push_back(y3); positions.push_back(z3);
	for (int i = 0; i < 4; ++i) {
		colors.push_back(r); colors.push_back(g); colors.push_back(b);
	}
	indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
	indices.push_back(base + 0); indices.push_back(base + 2); indices.push_back(base + 3);
}

void VoxelMeshGenerator::GenerateMesh(const VoxelData& data, Mesh& outMesh)
{
	std::vector<float> positions;
	std::vector<float> colors;
	std::vector<GLuint> indices;
	const float r = 0.5f, g = 0.5f, b = 0.5f;

	for (int z = 0; z < data.GetSizeZ(); ++z) {
		for (int y = 0; y < data.GetSizeY(); ++y) {
			for (int x = 0; x < data.GetSizeX(); ++x) {
				if (data.GetType(x, y, z) == VoxelType::Air) continue;

				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				float fz = static_cast<float>(z);

				if (!isSolid(data, x + 1, y, z)) {
					addQuad(positions, colors, indices,
						fx + 1, fy, fz, fx + 1, fy + 1, fz,
						fx + 1, fy + 1, fz + 1, fx + 1, fy, fz + 1, r, g, b);
				}
				if (!isSolid(data, x - 1, y, z)) {
					addQuad(positions, colors, indices,
						fx, fy, fz + 1, fx, fy + 1, fz + 1,
						fx, fy + 1, fz, fx, fy, fz, r, g, b);
				}
				if (!isSolid(data, x, y + 1, z)) {
					addQuad(positions, colors, indices,
						fx, fy + 1, fz, fx + 1, fy + 1, fz,
						fx + 1, fy + 1, fz + 1, fx, fy + 1, fz + 1, r, g, b);
				}
				if (!isSolid(data, x, y - 1, z)) {
					addQuad(positions, colors, indices,
						fx, fy, fz + 1, fx + 1, fy, fz + 1,
						fx + 1, fy, fz, fx, fy, fz, r, g, b);
				}
				if (!isSolid(data, x, y, z + 1)) {
					addQuad(positions, colors, indices,
						fx + 1, fy, fz + 1, fx + 1, fy + 1, fz + 1,
						fx, fy + 1, fz + 1, fx, fy, fz + 1, r, g, b);
				}
				if (!isSolid(data, x, y, z - 1)) {
					addQuad(positions, colors, indices,
						fx, fy, fz, fx + 1, fy, fz,
						fx + 1, fy + 1, fz, fx, fy + 1, fz, r, g, b);
				}
			}
		}
	}

	outMesh.Init(positions.data(), positions.size(),
		colors.data(), colors.size(),
		indices.data(), indices.size());
}
