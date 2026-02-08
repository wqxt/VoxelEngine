#pragma once
#include "render/Mesh.h"
#include "VoxelData.h"
#include <glad/glad.h>
#include <glm/ext/vector_int3.hpp>

class Chunk {
public:

	Chunk(int x, int y, int z);
	~Chunk();
	void Load(Mesh& mesh, VoxelData& voxelData);
	void Unload();
	void SetDirty();
	void RebuildDirty();
	Mesh* GetMesh() const { return m_Mesh; }
	const glm::ivec3& GetPosition() const { return m_position; }

private:
	glm::ivec3 m_position;
	VoxelData* m_VoxelData;
	Mesh* m_Mesh;
	bool m_IsDirty;

};