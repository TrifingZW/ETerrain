//
// Created by TrifingZW on 24-11-19.
//

#include "load_resources.h"

#include <e_terrain.h>

#include "core/helpers/assets_helper.h"

LoadResources::~LoadResources()
{
    delete mapLand;
    delete mapSea;
    delete Pen;
}

void LoadResources::Init()
{
    mapConfigParser.loadFromXml("config/def_map.xml");
    terrainConfigParser.loadFromXml("config/def_mapterrain.xml");

    LoadTexture2D();
}

void LoadResources::LoadTexture2D() const
{
    const std::string map_land = "map_land.png";
    const std::string map_sea = "map_sea.png";
    const std::string anim_status = "anim_status.png";
#ifdef PLATFORM_WINDOWS
    AssetsHelper::LoadTexture2DFromPath(*mapLand, map_land);
    AssetsHelper::LoadTexture2DFromPath(*mapSea, map_sea);
    AssetsHelper::LoadTexture2DFromPath(*AnimStatus, anim_status);
#elif defined(PLATFORM_ANDROID)
    AssetsHelper::LoadTexture2DFromAndroidAssets(*mapLand, map_land);
#endif
}
