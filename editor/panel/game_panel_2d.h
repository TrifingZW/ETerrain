//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include "core/graphics/render_target.h"
#include "core/graphics/sprite_batch.h"
#include "scene/node.h"
#include "scene/2d/camera_2d.h"
#include "scene/resources/shader.h"
#include "scene/resources/texture_2d.h"
#include "editor/parser/bin_parser.h"
#include "hex_manager.h"

class GamePanel2D : public Node
{
public:
    int width = 1, height = 1;
    Shader* shader = nullptr;
    Texture2D* colorUV = nullptr;
    HexVertexType* vertexInfo = nullptr;

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

    void NewFramebuffer(int width, int height);
    void GenerateOceanVertex() const;
};
