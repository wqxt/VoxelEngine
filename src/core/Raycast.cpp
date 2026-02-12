#include "Raycast.h"
#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>

RaycastHit Raycast::Cast(const glm::vec3& origin, const glm::vec3& direction, float maxDist,
	std::function<bool(int, int, int)> isSolid){


	RaycastHit result;
	result.hit = false;

	glm::vec3 dir = direction;
	float len = glm::length(dir);

	if (len < 1e-6f) {
		return result;
	}
	dir /= len;

	int ix = static_cast<int>(std::floor(origin.x));
	int iy = static_cast<int>(std::floor(origin.y));
	int iz = static_cast<int>(std::floor(origin.z));

	int stepX = dir.x >= 0 ? 1 : -1;
	int stepY = dir.y >= 0 ? 1 : -1;
	int stepZ = dir.z >= 0 ? 1 : -1;

	float tDeltaX = (std::abs(dir.x) >= 1e-6f) ? std::abs(1.f / dir.x) : 1e9f;
	float tDeltaY = (std::abs(dir.y) >= 1e-6f) ? std::abs(1.f / dir.y) : 1e9f;
	float tDeltaZ = (std::abs(dir.z) >= 1e-6f) ? std::abs(1.f / dir.z) : 1e9f;

	float tMaxX = (stepX > 0) ? (static_cast<float>(ix + 1) - origin.x) / dir.x : (static_cast<float>(ix) - origin.x) / dir.x;
	float tMaxY = (stepY > 0) ? (static_cast<float>(iy + 1) - origin.y) / dir.y : (static_cast<float>(iy) - origin.y) / dir.y;
	float tMaxZ = (stepZ > 0) ? (static_cast<float>(iz + 1) - origin.z) / dir.z : (static_cast<float>(iz) - origin.z) / dir.z;

	if (tMaxX < 0.f) tMaxX = 1e9f;
	if (tMaxY < 0.f) tMaxY = 1e9f;
	if (tMaxZ < 0.f) tMaxZ = 1e9f;

	float t = 0.f;
	int lastStepAxis = -1;
	const int maxSteps = 1024;

	for (int n = 0; n < maxSteps; ++n) {
		if (t > maxDist) {
			return result;
		}

		if (isSolid(ix, iy, iz)) {
			result.hit = true;
			result.blockPos = glm::ivec3(ix, iy, iz);

			if (lastStepAxis == 0) {
				result.normal = glm::vec3(-stepX, 0.f, 0.f);
			}
			else if (lastStepAxis == 1) {
				result.normal = glm::vec3(0.f, -stepY, 0.f);
			}
			else if (lastStepAxis == 2) {
				result.normal = glm::vec3(0.f, 0.f, -stepZ);
			}
			else {
				float nx = (dir.x >= 0.f) ? -1.f : 1.f;
				float ny = (dir.y >= 0.f) ? -1.f : 1.f;
				float nz = (dir.z >= 0.f) ? -1.f : 1.f;
				result.normal = glm::vec3(nx, ny, nz);
			}
			return result;
		}
		if (tMaxX < tMaxY && tMaxX < tMaxZ) {
			t = tMaxX;
			tMaxX += tDeltaX;
			ix += stepX;
			lastStepAxis = 0;
		} else if (tMaxY < tMaxZ) {
			t = tMaxY;
			tMaxY += tDeltaY;
			iy += stepY;
			lastStepAxis = 1;
		} else {
			t = tMaxZ;
			tMaxZ += tDeltaZ;
			iz += stepZ;
			lastStepAxis = 2;
		}
	}
	return result;
}