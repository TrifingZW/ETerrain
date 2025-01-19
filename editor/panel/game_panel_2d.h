//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <functional>

#include "core/graphics/render_target.h"
#include "core/graphics/sprite_batch.h"
#include "scene/node.h"
#include "scene/2d/camera_2d.h"
#include "scene/resources/shader.h"
#include "editor/parser/bin_parser.h"
#include "hex_manager.h"
#include "editor/world/land_unit.h"
#include "glm/detail/_noise.hpp"

class GamePanel2D : public Node
{
public:
    int width = 1, height = 1;
    Shader* shader = nullptr;
    HexVertexType* vertexInfo = nullptr;
    BufferManager* GridBufferManager = nullptr;
    LandUnit** landUnit = nullptr;

    Camera2D* camera2d = nullptr;
    RenderTarget* renderTarget = nullptr;

    BinParser binParser = {};
    HexManager* hexManager = nullptr;

    explicit GamePanel2D();
    ~GamePanel2D() override;

    void Init() override;
    void Ready() override;
    void Rendering(SpriteBatch& spriteBatch) override;
    void Gui() override;

    void ImageInput() const;
    void IterateLandUnit(const std::function<void(LandUnit&, glm::vec2)>& func) const;
    void NewFramebuffer(int width, int height);
    void GenerateOceanVertex() const;
    void GenerateBuffer() const;

    static void DrawTerrain(SpriteBatch& spriteBatch, uint8_t type, uint8_t id, const glm::vec2& position);
};
