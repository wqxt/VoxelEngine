#include <iostream>
#include <string>
#include "map/MapData.h"
#pragma once

class MapLoader {
public:
	MapLoader() = default;
	~MapLoader() = default;

	void LoadMap(const std::string& filename, MapData& outMap);
};