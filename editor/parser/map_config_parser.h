//
// Created by TrifingZW on 24-11-19.
//

#pragma once

#include <string>
#include <vector>

struct Map
{
    int id;
    std::string name;
    std::string file;
    int w;
    int h;
    std::string tile;
    std::string texPos;
    int tileSize;
    std::string pattern;
    int patternSize;
};

class MapConfigParser
{
public:
    std::vector<Map> mapArray;

    bool loadFromXml(const std::string& filePath);
};
