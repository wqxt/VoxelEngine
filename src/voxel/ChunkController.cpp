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
	m_chunks.reserve(static_cast<size_t>(gridX) * static_cast<size_t>(gridY) * static_cast<size_t>(gridZ));

	for (int z = 0; z < gridZ; ++z)
	{
		for (int y = 0; y < gridY; ++y)
		{
			for (int x = 0; x < gridX; ++x)
			{
				m_chunks.emplace_back(x, y, z);
			}
		}
	}
}

size_t ChunkController::getIndex(int gridX, int gridY, int gridZ) const
{
	return static_cast<size_t>(gridX)
		+ static_cast<size_t>(gridY) * static_cast<size_t>(m_gridX)
		+ static_cast<size_t>(gridZ) * static_cast<size_t>(m_gridX)
		* static_cast<size_t>(m_gridY);
}


bool ChunkController::IsSolid(int wx, int wy, int wz) const
{
	if (wx < 0 || wx >= m_worldSizeX ||
		wy < 0 || wy >= m_worldSizeY ||
		wz < 0 || wz >= m_worldSizeZ)
	{

		return false;
	}

	int cx = wx / m_chunkSizeX;
	int cy = wy / m_chunkSizeY;
	int cz = wz / m_chunkSizeZ;

	if (cx < 0 || cx >= m_gridX || 
		cy < 0 || cy >= m_gridY || 
		cz < 0 || cz >= m_gridZ)
	{
		return false;
	}

	int lx = wx % m_chunkSizeX;
	int ly = wy % m_chunkSizeY;
	int lz = wz % m_chunkSizeZ;

	size_t idx = static_cast<size_t>(cx) + static_cast<size_t>(cy) * static_cast<size_t>(m_gridX)
		+ static_cast<size_t>(cz) * static_cast<size_t>(m_gridX) * static_cast<size_t>(m_gridY);


	if (idx >= m_voxelDatas.size())
	{
		return false;
	}

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

	m_numChunks = static_cast<size_t>(m_gridX) * static_cast<size_t>(m_gridY) * static_cast<size_t>(m_gridZ);
	m_voxelDatas.reserve(m_numChunks);
	m_meshes.reserve(m_numChunks);

	for (int z = 0; z < m_gridZ; ++z)
	{
		for (int y = 0; y < m_gridY; ++y)
		{
			for (int x = 0; x < m_gridX; ++x)
			{
				m_voxelDatas.emplace_back(chunkSizeX, chunkSizeY, chunkSizeZ);
				VoxelData& voxelData = m_voxelDatas.back();
				for (int cz = 0; cz < chunkSizeZ; ++cz)
				{
					for (int cy = 0; cy < chunkSizeY; ++cy)
					{
						for (int cx = 0; cx < chunkSizeX; ++cx)
						{
							if (cy == 0)
							{
								voxelData.SetType(cx, cy, cz, VoxelType::Stone);
							}
							else if (cy == chunkSizeY - 1)
							{
								voxelData.SetType(cx, cy, cz, VoxelType::Grass);
							}
							else
							{
								voxelData.SetType(cx, cy, cz, VoxelType::Dirt);
							}
						}
					}
				}

				m_meshes.emplace_back();
				int worldX = x * m_chunkSizeX;
				int worldY = y * m_chunkSizeY;
				int worldZ = z * m_chunkSizeZ;
				auto callback = [this](int wx, int wy, int wz) { return this->IsSolid(wx, wy, wz); };
				VoxelMeshGenerator::GenerateMesh(voxelData, m_meshes.back(), worldX, worldY, worldZ, callback);
				LoadChunk(x, y, z, m_meshes.back(), m_voxelDatas.back());
			}
		}
	}
}

void ChunkController::SetVoxelType(int chunkBlockNumX, int chunkBlockNumY, int chunkBlockNumZ, uint8_t type, size_t linearChunkIndex)
{
	if (linearChunkIndex >= m_voxelDatas.size())
	{
		return;
	}

	if (chunkBlockNumX < 0 || chunkBlockNumX >= m_chunkSizeX ||
		chunkBlockNumY < 0 || chunkBlockNumY >= m_chunkSizeY ||
		chunkBlockNumZ < 0 || chunkBlockNumZ >= m_chunkSizeZ)
	{
		return;
	}
	m_voxelDatas[linearChunkIndex].SetType(chunkBlockNumX, chunkBlockNumY, chunkBlockNumZ, type);
}

void ChunkController::DestroyBlock(int worldX, int worldY, int worldZ)
{
	if (worldX < 0 || worldX >= m_worldSizeX ||
		worldY < 0 || worldY >= m_worldSizeY ||
		worldZ < 0 || worldZ >= m_worldSizeZ)
	{

		return;
	}

	int cx = worldX / m_chunkSizeX;
	int cy = worldY / m_chunkSizeY;
	int cz = worldZ / m_chunkSizeZ;

	int lx = worldX % m_chunkSizeX;
	int ly = worldY % m_chunkSizeY;
	int lz = worldZ % m_chunkSizeZ;
	size_t idx = static_cast<size_t>(cx) + static_cast<size_t>(cy) * static_cast<size_t>(m_gridX)
		+ static_cast<size_t>(cz) * static_cast<size_t>(m_gridX) * static_cast<size_t>(m_gridY);

	if (idx >= m_voxelDatas.size())
	{
		return;
	}
	m_voxelDatas[idx].SetType(lx, ly, lz, VoxelType::Air);
	SetChunkDirty(cx, cy, cz);
	RebuildChunk(cx, cy, cz);
	RebuildChunk(cx - 1, cy, cz);
	RebuildChunk(cx + 1, cy, cz);
	RebuildChunk(cx, cy - 1, cz);
	RebuildChunk(cx, cy + 1, cz);
	RebuildChunk(cx, cy, cz - 1);
	RebuildChunk(cx, cy, cz + 1);
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

	if (idx >= m_meshes.size() || idx >= m_chunks.size() || idx >= m_voxelDatas.size())
	{
		return;
	}

	int worldX = gridX * m_chunkSizeX;
	int worldY = gridY * m_chunkSizeY;
	int worldZ = gridZ * m_chunkSizeZ;
	auto callback = [this](int wx, int wy, int wz) { return this->IsSolid(wx, wy, wz); };
	VoxelMeshGenerator::GenerateMesh(m_voxelDatas[idx], m_meshes[idx], worldX, worldY, worldZ, callback);
	m_chunks[idx].SetDirty();

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
	for (int z = 0; z < m_gridZ; ++z)
	{
		for (int y = 0; y < m_gridY; ++y)
		{
			for (int x = 0; x < m_gridX; ++x)
			{
				size_t idx = getIndex(x, y, z);
				if (idx < m_chunks.size() && m_chunks[idx].GetMesh() != nullptr)
				{
					RebuildChunk(x, y, z);
				}
			}
		}
	}
}