#pragma once

#include "VoxelData.h"
#include <functional>

class Mesh;

class VoxelMeshGenerator {

public:
    static void GenerateMesh(const VoxelData& data, Mesh& outMesh,
        int chunkWorldX, int chunkWorldY, int chunkWorldZ,
        std::function<bool(int, int, int)> checkNeighborChunkFunc);
};