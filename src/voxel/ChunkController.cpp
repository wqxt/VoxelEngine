#include "ChunkController.h"

ChunkController::ChunkController()
	: m_gridX(0), m_gridY(0), m_gridZ(0)
{
}

ChunkController::~ChunkController()
{
}

void ChunkController::Init(int gridX, int gridY, int gridZ)
{
	m_gridX = gridX;
	m_gridY = gridY;
	m_gridZ = gridZ;
	m_chunks.reserve(static_cast<size_t>(gridX) * gridY * gridZ);
	for (int z = 0; z < gridZ; ++z) {
		for (int y = 0; y < gridY; ++y) {
			for (int x = 0; x < gridX; ++x) {
				m_chunks.emplace_back(x, y, z);
			}
		}
	}
}

size_t ChunkController::getIndex(int gridX, int gridY, int gridZ) const
{
	return static_cast<size_t>(gridX + gridY * m_gridX + gridZ * m_gridX * m_gridY);
}

void ChunkController::LoadChunk(int gridX, int gridY, int gridZ, Mesh& mesh, VoxelData& voxelData)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
		return;
	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].Load(mesh, voxelData);
}

void ChunkController::RebuildChunk(int gridX, int gridY, int gridZ)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
		return;
	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].RebuildDirty();
}

void ChunkController::UnloadChunk(int gridX, int gridY, int gridZ)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
		return;
	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].Unload();
}

void ChunkController::GetChunkMesh(int gridX, int gridY, int gridZ, Mesh*& outMesh)
{
	outMesh = nullptr;
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
		return;
	size_t idx = getIndex(gridX, gridY, gridZ);
	outMesh = m_chunks[idx].GetMesh();
}

void ChunkController::GetChunkPosition(int gridX, int gridY, int gridZ, glm::ivec3& outPosition)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
		return;
	size_t idx = getIndex(gridX, gridY, gridZ);
	outPosition = m_chunks[idx].GetPosition();
}

const std::vector<Chunk>& ChunkController::GetChunks() const
{
	return m_chunks;
}

void ChunkController::RebuildAllDirtyChunks()
{
	for (Chunk& chunk : m_chunks) {
		chunk.RebuildDirty();
	}
}