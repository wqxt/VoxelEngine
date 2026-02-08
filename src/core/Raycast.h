#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <functional>

struct RaycastHit {
	bool hit;
	glm::ivec3 blockPos;
	glm::vec3 normal;
};

class Raycast {
public:
	static RaycastHit Cast(const glm::vec3& origin, const glm::vec3& direction, float maxDist,
		std::function<bool(int, int, int)> isSolid);
};
