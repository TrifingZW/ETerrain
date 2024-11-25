//
// Created by TrifingZW on 24-11-19.
//

#pragma once
#include "node.h"
#include "parser/map_config_parser.h"
#include "parser/resource_texture_parser.h"
#include "parser/terrain_config_parser.h"


class LoadResources : public Node {
public:
    MapConfigParser mapConfigParser{};
    TerrainConfigParser terrainConfigParser{};
    ResourceTextureParser plantResourceTextureParser = ResourceTextureParser("assets/plant_hd");
    ResourceTextureParser terrainResourceTextureParser = ResourceTextureParser("assets/terrain_hd");

    void Init() override;
};
