#include "MapData.h"

MapData::MapData() : m_sizeX(0), m_sizeY(0), m_sizeZ(0){}

MapData :: ~MapData()
{
	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeZ = 0;
	m_cells.clear();
}

uint8_t MapData::GetCell(int x, int y, int z)
{
	if (x < 0 || x >= m_sizeX ||
		y < 0 || y >= m_sizeY ||
		z < 0 || z >= m_sizeZ)
	{
		return 0;
	}

	return m_cells[x + z * m_sizeX + y * m_sizeX * m_sizeZ];
}

void MapData::SetCell(int x, int y, int z, uint8_t value)
{
	if (x < 0 || x >= m_sizeX ||
		y < 0 || y >= m_sizeY ||
		z < 0 || z >= m_sizeZ)
	{
		return;
	}

	m_cells[x + z * m_sizeX + y * m_sizeX * m_sizeZ] = value;
}

void MapData::Resize(int newSizeX, int newSizeY, int newSizeZ)
{
	if (newSizeX < 0 || newSizeY < 0 || newSizeZ < 0)
	{
		std::cerr << "Error: could not open file" << std::endl;
		return;
	}

	m_sizeX = newSizeX;
	m_sizeY = newSizeY;
	m_sizeZ = newSizeZ;
	m_cells.resize(static_cast<size_t>(m_sizeX) * static_cast<size_t>(m_sizeY) * static_cast<size_t>(m_sizeZ), 0);
}