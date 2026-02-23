#pragma once
#include <iostream>
#include <vector>

class MapData {

public:
	MapData();
	~MapData();
	void GetSizeX(int& outSizeX) const { outSizeX = m_sizeX; }
	void GetSizeY(int& outSizeY) const { outSizeY = m_sizeY; }
	void GetSizeZ(int& outSizeZ) const { outSizeZ = m_sizeZ; }

	uint8_t GetCell(int x, int y, int z);
	void SetCell(int x, int y, int z, uint8_t value);
	void Resize(int newSizeX, int newSizeY, int newSizeZ);
private:

	int m_sizeX;
	int m_sizeY;
	int m_sizeZ;

	std::vector<uint8_t> m_cells;
};