#pragma once
#include <cstdint>

namespace VoxelType {
	constexpr uint8_t Air = 0;
	constexpr uint8_t Stone = 1;
	constexpr uint8_t Dirt = 2;
	constexpr uint8_t Grass = 3;
}

struct Voxel {
	uint8_t type;
};