//
// Created by TrifingZW on 24-11-19.
//

#pragma once

#include "scene/node.h"
#include "parser/map_config_parser.h"
#include "parser/resource_texture_parser.h"
#include "parser/terrain_config_parser.h"

class LoadResources : public Node
{
public:
    MapConfigParser mapConfigParser{};
    TerrainConfigParser terrainConfigParser{};
    ResourceTextureParser plantResourceTextureParser = ResourceTextureParser("plant_hd");
    ResourceTextureParser terrainResourceTextureParser = ResourceTextureParser("terrain_hd");

    Texture2D* mapLand = new Texture2D();
    Texture2D* mapSea = new Texture2D();

    ~LoadResources() override;

    void Init() override;
    void LoadTexture2D() const;
};
