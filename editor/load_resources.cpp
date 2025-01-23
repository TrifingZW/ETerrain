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
    const std::string anim_status = "anim_status.png";
#ifdef PLATFORM_WINDOWS
    ImGuiHelper::LoadTexture2DFromPath(*mapLand, map_land);
    ImGuiHelper::LoadTexture2DFromPath(*AnimStatus, anim_status);
    ImGuiHelper::LoadSVGToTexture2DFromFile(*Pen, "pen.svg", ETerrain::DpiScale);
#elif defined(PLATFORM_ANDROID)
    Helper::LoadTexture2DFromAndroidAssets(*mapLand, texturePath);
#endif
}
