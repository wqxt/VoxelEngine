#include "Voxel.h"


Voxel::Voxel() : m_x(0), m_y(0), m_z(0) {
}

Voxel::~Voxel() {

}

void Voxel::SetPosition(int& x, int& y, int& z) {
	m_x = x;
	m_y = y;
	m_z = z;

}
glm::ivec3 Voxel::GetPosition() const {
	return glm::ivec3(m_x, m_y, m_z);
}