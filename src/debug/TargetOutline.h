#pragma once

#include "render/Mesh.h"

class Camera;
class Render;
class Shader;

class TargetOutline {
public:
	TargetOutline();
	~TargetOutline();
	void Init();
	void SetTransform(float& posX, float& posY, float& posZ);
	void Draw(const Camera& camera, Render& renderer, Shader& shader);

private:

	float m_posX;
	float m_posY;
	float m_posZ;
	Mesh m_mesh;
};