#include <iostream>
#pragma once

class MapData {
	public:
	MapData();
	void GetSizeX(int& outSizeX) const { outSizeX = m_sizeX; }
	void GetSizeY(int& outSizeY) const { outSizeY = m_sizeY; }
	void GetSizeZ(int& outSizeZ) const { outSizeZ = m_sizeZ; }


private:
	
	int m_sizeX;
	int m_sizeY;
	int m_sizeZ;

};