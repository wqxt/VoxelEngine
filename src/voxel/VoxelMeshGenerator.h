#pragma once

#include "VoxelData.h"
#include <functional>

class Mesh;

class VoxelMeshGenerator
{
public:
    static void GenerateMesh(const VoxelData &data, Mesh &outMesh,
                             int chunkWorldX, int chunkWorldY, int chunkWorldZ,
                             std::function<bool(int, int, int)> checkNeighborChunkFunc);

private:
    static void GenerateGreedyMesh(const VoxelData &data, Mesh &outMesh,
                                   int chunkWorldX, int chunkWorldY, int chunkWorldZ,
                                   std::function<bool(int, int, int)> checkNeighborChunkFunc,
                                   int axis, int direction,
                                   std::vector<float> positions,
                                   std::vector<float> colors,
                                   std::vector<float> uvs,
                                   std::vector<GLuint> indices);
};