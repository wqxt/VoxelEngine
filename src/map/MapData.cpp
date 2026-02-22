#include "MapData.h"

MapData::MapData() : m_sizeX(0), m_sizeZ(0){}

MapData :: ~MapData()
{
	m_sizeX = 0;
	m_sizeZ = 0;
	m_cells.clear();
}

uint8_t MapData::GetCell(int x, int z)
{
	if (x < 0 || x >= m_sizeX ||
		z < 0 || z >= m_sizeZ)
	{
		return 0;
	}

	return m_cells[x + z * m_sizeX];
}

void MapData::SetCell(int x, int z, uint8_t value)
{
	if (x < 0 || x >= m_sizeX ||
		z < 0 || z >= m_sizeZ)
	{
		return;
	}

	m_cells[x + z * m_sizeX] = value;
}	

void MapData::Resize(int newSizeX, int newSizeZ)
{
	if (newSizeX < 0 || newSizeZ < 0)
	{
		std::cerr << "Error: could not open file" << std::endl;
		return;
	}

	m_sizeX = newSizeX;
	m_sizeZ = newSizeZ;
	m_cells.resize(static_cast<size_t>(m_sizeX) * static_cast<size_t>(m_sizeZ), 0);
}