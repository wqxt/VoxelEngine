#include "VoxelData.h"


VoxelData::VoxelData(int sizeX, int sizeY, int sizeZ): m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ) {
	m_data.resize(sizeX * sizeY * sizeZ);
}

uint8_t VoxelData::GetType(int x, int y, int z) const {
	return m_data[x + y * m_sizeX + z * m_sizeX * m_sizeY].type;
}

void VoxelData::SetType(int x, int y, int z, uint8_t type) {
	m_data[x + y * m_sizeX + z * m_sizeX * m_sizeY].type = type;
}

int VoxelData::GetSizeX() const {
	return m_sizeX;
}

int VoxelData::GetSizeY() const {
	return m_sizeY;
}

int VoxelData::GetSizeZ() const {
	return m_sizeZ;
}