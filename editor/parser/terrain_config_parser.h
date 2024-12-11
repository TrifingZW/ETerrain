//
// Created by TrifingZW on 24-11-19.
//

#pragma once

#include <string>
#include <vector>

struct Tile
{
    int idx;
    std::string image;
};

struct Terrain
{
    std::string name;
    int terrainG;
    int type;
    std::vector<Tile> tiles;
};

class TerrainConfigParser
{
public:
    std::vector<Terrain> terrains;

    bool loadFromXml(const std::string& filePath);
};
