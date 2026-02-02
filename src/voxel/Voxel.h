#pragma once

#include <vector>
#include <glm/glm.hpp>

class Voxel {

public:
	Voxel();
	~Voxel();
	void SetPosition(int& x, int& y, int& z);
	glm::ivec3 GetPosition() const;

private:
	int m_x;
	int m_y;
	int m_z;

};