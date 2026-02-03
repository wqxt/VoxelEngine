#pragma once

#include "VoxelData.h"

class Mesh;

class VoxelMeshGenerator {

public:
	static void GenerateMesh(const VoxelData& data, Mesh& outMesh);
};
