#pragma once
#include "Chunk.h"
#include <vector>
#include "VoxelData.h"
#include <cstddef>
#include <glm/glm.hpp>
#include <functional>

class ChunkController {

public:
	ChunkController();
	~ChunkController();

	void InitGrid(int gridX, int gridY, int gridZ);
	void SetupChunks(int chunkSizeX, int chunkSizeY, int chunkSizeZ);
	void LoadChunk(int gridX, int gridY, int gridZ, Mesh& mesh, VoxelData& voxelData);
	void SetChunkDirty(int gridX, int gridY, int gridZ);
	void RebuildChunk(int gridX, int gridY, int gridZ);
	void UnloadChunk(int gridX, int gridY, int gridZ);
	void GetChunkMesh(int gridX, int gridY, int gridZ, Mesh*& outMesh);
	void GetChunkPosition(int gridX, int gridY, int gridZ, glm::ivec3& outPosition);
	void RebuildAllDirtyChunks();
	const std::vector<Chunk>& GetChunks() const;
	std::function<bool(int, int, int)> GetIsSolidCallback() const;
	bool IsSolid(int wx, int wy, int wz) const;
	void SetVoxelType(int chunkBlockNumX, int chunkBlockNumY, int chunkBlockNumZ, uint8_t type, size_t linearChunkIndex);
	void DestroyBlock(int worldX, int worldY, int worldZ);

	int GetChunkSizeX() const { return m_chunkSizeX; }
	int GetChunkSizeY() const { return m_chunkSizeY; }
	int GetChunkSizeZ() const { return m_chunkSizeZ; }

private:


	std::vector<VoxelData> m_voxelDatas;
	std::vector<Chunk> m_chunks;
	std::vector<Mesh> m_meshes;
	size_t m_numChunks = 0;

	int m_chunkSizeX;
	int m_chunkSizeY;
	int m_chunkSizeZ;
	int m_gridX;
	int m_gridY;
	int m_gridZ;

	int m_worldSizeX;
	int m_worldSizeY;
	int m_worldSizeZ;
	size_t getIndex(int gridX, int gridY, int gridZ) const;
};