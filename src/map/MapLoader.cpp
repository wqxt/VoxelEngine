#include "MapLoader.h"
#include<fstream>


void MapLoader::LoadMap(const std::string& filename, MapData& outMap) {

	std::ifstream map(filename);

	if (map.is_open() == false)
	{
		std::cerr << "Error: could not open file" << std::endl;
	}
	else
	{
		std::cout << "File reading is succes" << std::endl;
	}

	std::string mapLine;
	int sizeZ = 0;
	int sizeX = 0;

	std::vector<std::string> mapLines;

	while (std::getline(map, mapLine))
	{
		if (mapLine.empty())
		{
			continue;
		}

		if (mapLine[0] == '/')
		{
			break;
		}

		mapLines.push_back(mapLine);
	}

	sizeX = static_cast<int>(mapLines[0].size());
	sizeZ = static_cast<int>(mapLines.size());
	outMap.Resize(sizeX, sizeZ);

	for (int x = 0; x < mapLines.size(); ++x)
	{
		for (int z = 0; z < mapLines[x].size(); ++z)
		{
			char c = mapLines[x][z];
		
			if (c == '#')
			{
				outMap.SetCell(static_cast<int>(z), static_cast<int>(x), 1);
			}
			else if (c == '.')
			{
				outMap.SetCell(static_cast<int>(z), static_cast<int>(x), 3);
			}
			else
			{
				outMap.SetCell(static_cast<int>(z), static_cast<int>(x), 0);

			}
		}
	}
}