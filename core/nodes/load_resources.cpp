//
// Created by TrifingZW on 24-11-19.
//

#include "load_resources.h"

void LoadResources::Init()
{
    mapConfigParser.loadFromXml("assets/config/def_map.xml");
    terrainConfigParser.loadFromXml("assets/config/def_mapterrain.xml");

    LoadTexture2D();
}

void LoadResources::LoadTexture2D() const
{
    const std::string texturePath = "map_land.png";
    Helper::LoadTexture2D(*mapLand, texturePath);
}
