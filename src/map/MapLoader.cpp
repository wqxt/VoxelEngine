#include "MapLoader.h"
#include <fstream>

void MapLoader::LoadMap(const std::string& filename, MapData& outMap)
{
	std::ifstream map(filename);

	if (map.is_open() == false)
	{
		std::cerr << "Error: could not open file" << std::endl;
		return;
	}

	std::cout << "File reading is succes" << std::endl;

	std::string line;
	std::vector<std::string> allLines;

	while (std::getline(map, line))
	{
		if (line.empty())
			continue;
		if (line[0] == '/')
			break;
		allLines.push_back(line);
	}

	std::vector<std::vector<std::string>> layers;
	layers.push_back({});

	for (const std::string& l : allLines)
	{
		if (l.size() >= 3 && l[0] == '-' && l[1] == '-' && l[2] == '-')
		{
			layers.push_back({});
			continue;
		}
		layers.back().push_back(l);
	}

	while (layers.size() > 1 && layers.back().empty())
		layers.pop_back();

	if (layers.empty() || layers[0].empty())
		return;

	int sizeX = static_cast<int>(layers[0][0].size());
	int sizeZ = static_cast<int>(layers[0].size());
	int sizeY = static_cast<int>(layers.size());
	outMap.Resize(sizeX, sizeY, sizeZ);

	for (int y = 0; y < sizeY; ++y)
	{
		const std::vector<std::string>& block = layers[y];
		for (int z = 0; z < sizeZ && z < static_cast<int>(block.size()); ++z)
		{
			const std::string& row = block[z];
			for (int x = 0; x < sizeX && x < static_cast<int>(row.size()); ++x)
			{
				char c = row[x];
				uint8_t value = (c == '#') ? 1 : (c == '.') ? 2 : 0;
				outMap.SetCell(x, y, z, value);
			}
		}
	}
}