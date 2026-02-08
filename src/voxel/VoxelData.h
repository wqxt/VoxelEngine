#pragma once
#include "Voxel.h"
#include <vector>

class VoxelData {

public:

	VoxelData(int sizeX, int sizeY, int sizeZ);
	uint8_t GetType(int x, int y, int z) const;
	void SetType(int x, int y, int z, uint8_t type);
	int GetSizeX() const;
	int GetSizeY() const;
	int GetSizeZ() const;

private:
	std::vector<Voxel> m_data;
	int m_sizeX;
	int m_sizeY;
	int m_sizeZ;
};
