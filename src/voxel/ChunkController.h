#pragma once
#include "Chunk.h"
#include <vector>
#include "VoxelData.h"
#include <cstddef>
#include <glm/glm.hpp>

class ChunkController {

public:
	ChunkController();
	~ChunkController();

	void Init(int gridX, int gridY, int gridZ);
	void LoadChunk(int gridX, int gridY, int gridZ, Mesh& mesh, VoxelData& voxelData);
	void RebuildChunk(int gridX, int gridY, int gridZ);
	void UnloadChunk(int gridX, int gridY, int gridZ);
	void GetChunkMesh(int gridX, int gridY, int gridZ, Mesh*& outMesh);
	void GetChunkPosition(int gridX, int gridY, int gridZ, glm::ivec3& outPosition);
	void RebuildAllDirtyChunks();
	const std::vector<Chunk>& GetChunks() const;

private:
	size_t getIndex(int gridX, int gridY, int gridZ) const;

	std::vector<Chunk> m_chunks;
	int m_gridX;
	int m_gridY;
	int m_gridZ;
};