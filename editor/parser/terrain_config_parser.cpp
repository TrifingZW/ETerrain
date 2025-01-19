//
// Created by TrifingZW on 24-11-19.
//

#include "terrain_config_parser.h"

#include <pugixml.hpp>
#include <stdexcept>

bool TerrainConfigParser::loadFromXml(const std::string& filePath)
{
    pugi::xml_document doc;
#ifdef PLATFORM_WINDOWS
    if (const pugi::xml_parse_result result = doc.load_file(("assets/" + filePath).c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#elif defined(PLATFORM_ANDROID)
    std::string xmlData;
    Helper::LoadStringFromAndroidAssets(xmlData, filePath);
    if (const pugi::xml_parse_result result = doc.load_string(xmlData.c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#endif

    const auto terrainsNode = doc.child("terrains");
    if (!terrainsNode)
    {
        throw std::runtime_error("No <terrains> root element found");
    }

    for (auto terrainNode: terrainsNode.children("terrain"))
    {
        Terrain terrain;
        terrain.name = terrainNode.attribute("name").as_string();
        terrain.terrainG = terrainNode.attribute("terrain").as_int();
        terrain.type = terrainNode.attribute("type").as_int();

        // Parse <tile> elements
        for (auto tileNode: terrainNode.children("tile"))
        {
            Tile tile;
            tile.idx = tileNode.attribute("idx").as_int();
            tile.image = tileNode.attribute("image").as_string();
            terrain.tiles.push_back(tile);
        }

        terrains.push_back(terrain);
    }

    return true;
}
