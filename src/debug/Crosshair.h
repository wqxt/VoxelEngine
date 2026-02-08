#pragma once

#include "render/Mesh.h"

class Camera;
class Render;
class Shader;

class Crosshair {
public:
	Crosshair();
	~Crosshair();
	void Init();
	void Draw(const Camera& camera, Render& renderer, Shader& shader);

private:
	Mesh m_mesh;
};
