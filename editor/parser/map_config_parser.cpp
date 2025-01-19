//
// Created by TrifingZW on 24-11-19.
//

#include "map_config_parser.h"

#include <pugixml.hpp>
#include <stdexcept>

bool MapConfigParser::loadFromXml(const std::string& filePath)
{
    pugi::xml_document doc;
#ifdef PLATFORM_WINDOWS
    if (const pugi::xml_parse_result result = doc.load_file(("assets/" + filePath).c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#elif defined( PLATFORM_ANDROID)
    std::string xmlData;
    Helper::LoadStringFromAndroidAssets(xmlData, filePath);
    if (const pugi::xml_parse_result result = doc.load_string(xmlData.c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#endif

    const auto mapsNode = doc.child("maps");
    if (!mapsNode)
    {
        throw std::runtime_error("No <maps> root element found");
    }

    for (auto mapNode: mapsNode.children("map"))
    {
        Map map;
        map.id = mapNode.attribute("id").as_int();
        map.name = mapNode.attribute("name").as_string();
        map.file = mapNode.attribute("file").as_string();
        map.w = mapNode.attribute("w").as_int();
        map.h = mapNode.attribute("h").as_int();
        map.tile = mapNode.attribute("tile").as_string();
        map.texPos = mapNode.attribute("textpos").as_string();
        map.tileSize = mapNode.attribute("tilesize").as_int();
        map.pattern = mapNode.attribute("pattern").as_string();
        map.patternSize = mapNode.attribute("patternsize").as_int();

        mapArray.push_back(map);
    }

    return true;
}
