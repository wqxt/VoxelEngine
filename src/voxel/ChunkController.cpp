#include "ChunkController.h"
#include "VoxelMeshGenerator.h"

ChunkController::ChunkController() :
	m_numChunks(0),
	m_chunkSizeX(0),
	m_chunkSizeY(0),
	m_chunkSizeZ(0),
	m_gridX(0),
	m_gridY(0),
	m_gridZ(0),
	m_worldSizeX(0),
	m_worldSizeY(0),
	m_worldSizeZ(0)
{
}

ChunkController::~ChunkController()
{
}

void ChunkController::InitGrid(int gridX, int gridY, int gridZ)
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


bool ChunkController::IsSolid(int wx, int wy, int wz) const
{
	if (wx < 0 || wx >= m_worldSizeX || wy < 0 || wy >= m_worldSizeY || wz < 0 || wz >= m_worldSizeZ)
		return false;

	int cx = wx / m_chunkSizeX;
	int cy = wy / m_chunkSizeY;
	int cz = wz / m_chunkSizeZ;
	int lx = wx % m_chunkSizeX;
	int ly = wy % m_chunkSizeY;
	int lz = wz % m_chunkSizeZ;
	size_t idx = static_cast<size_t>(cx + cy * m_gridX + cz * m_gridX * m_gridY);
	return m_voxelDatas[idx].GetType(lx, ly, lz) != VoxelType::Air;
};

void ChunkController::SetupChunks(int chunkSizeX, int chunkSizeY, int chunkSizeZ)
{
	m_chunkSizeX = chunkSizeX;
	m_chunkSizeY = chunkSizeY;
	m_chunkSizeZ = chunkSizeZ;


	m_worldSizeX = m_gridX * m_chunkSizeX;
	m_worldSizeY = m_gridY * m_chunkSizeY;
	m_worldSizeZ = m_gridZ * m_chunkSizeZ;

	m_numChunks = m_gridX * m_gridY * m_gridZ;

	for (size_t i = 0; i < m_numChunks; ++i)
	{
		m_voxelDatas.emplace_back(chunkSizeX, chunkSizeY, chunkSizeZ);
		VoxelData& vd = m_voxelDatas.back();
		for (int z = 0; z < chunkSizeZ; ++z)
		{
			for (int y = 0; y < chunkSizeY; ++y)
			{
				for (int x = 0; x < chunkSizeX; ++x)
				{
					if (y == 0)
					{
						vd.SetType(x, y, z, VoxelType::Stone);
					}
					else if (y == chunkSizeY - 1)
					{
						vd.SetType(x, y, z, VoxelType::Grass);
					}
					else
					{
						vd.SetType(x, y, z, VoxelType::Dirt);
					}
				}
			}
		}
		m_meshes.emplace_back();
		VoxelMeshGenerator::GenerateMesh(vd, m_meshes.back());


	}

	for (int z = 0; z < m_gridZ; ++z) 
	{
		for (int y = 0; y < m_gridY; ++y) 
		{
			for (int x = 0; x < m_gridX; ++x) 
			{
				size_t idx = static_cast<size_t>(x + y * m_gridX + z * m_gridX * m_gridY);
				LoadChunk(x, y, z, m_meshes[idx], m_voxelDatas[idx]);
			}
		}
	}
}
void ChunkController::SetVoxelType(int chunkBlockNumX, int chunkBlockNumY, int chunkBlockNumZ, uint8_t type, size_t linearChunkIndex)
{
	m_voxelDatas[linearChunkIndex].SetType(chunkBlockNumX, chunkBlockNumY, chunkBlockNumZ, type);

}

void ChunkController::LoadChunk(int gridX, int gridY, int gridZ, Mesh& mesh, VoxelData& voxelData)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}
	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].Load(mesh, voxelData);
}

void ChunkController::SetChunkDirty(int gridX, int gridY, int gridZ)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}
	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].SetDirty();
}

void ChunkController::RebuildChunk(int gridX, int gridY, int gridZ)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}

	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].RebuildDirty();
}

void ChunkController::UnloadChunk(int gridX, int gridY, int gridZ)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}

	size_t idx = getIndex(gridX, gridY, gridZ);
	m_chunks[idx].Unload();
}

void ChunkController::GetChunkMesh(int gridX, int gridY, int gridZ, Mesh*& outMesh)
{
	outMesh = nullptr;
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}

	size_t idx = getIndex(gridX, gridY, gridZ);
	outMesh = m_chunks[idx].GetMesh();
}

void ChunkController::GetChunkPosition(int gridX, int gridY, int gridZ, glm::ivec3& outPosition)
{
	if (gridX < 0 || gridX >= m_gridX || gridY < 0 || gridY >= m_gridY || gridZ < 0 || gridZ >= m_gridZ)
	{
		return;
	}

	size_t idx = getIndex(gridX, gridY, gridZ);
	outPosition = m_chunks[idx].GetPosition();
}

const std::vector<Chunk>& ChunkController::GetChunks() const
{
	return m_chunks;
}

std::function<bool(int, int, int)> ChunkController::GetIsSolidCallback() const
{
	return [this](int x, int y, int z) { return IsSolid(x, y, z); };
}
void ChunkController::RebuildAllDirtyChunks()
{
	for (Chunk& chunk : m_chunks) {
		chunk.RebuildDirty();
	}
}