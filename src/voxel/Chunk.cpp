#include "Chunk.h"
#include "VoxelMeshGenerator.h"

Chunk::Chunk(int x, int y, int z) :
	m_position(x, y, z), m_VoxelData(nullptr), m_Mesh(nullptr), m_IsDirty(){}

Chunk::~Chunk()
{
	Chunk::Unload();
}

void Chunk::Load(Mesh& mesh, VoxelData& voxelData)
{
	m_Mesh = &mesh;
	m_VoxelData = &voxelData;
}

void Chunk::Unload()
{
	m_VoxelData = nullptr;
	m_Mesh = nullptr;
}

void Chunk::SetDirty()
{
	m_IsDirty = true;
}

void Chunk::RebuildDirty()
{
	m_IsDirty = false;
}