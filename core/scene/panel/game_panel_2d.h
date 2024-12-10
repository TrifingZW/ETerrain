//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include "hex_manager.h"
#include "node.h"
#include "parser/bin_parser.h"
#include "scene/2d/camera_2d.h"
#include "scene/resources/shader.h"


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

    explicit GamePanel2D()
    {
        auto* camera = new Camera2D();
        camera2d = camera;
        AddChild(std::unique_ptr<Node>(camera));
    }

    void Init() override;
    void Ready() override;
    void Rendering(SpriteBatch& spriteBatch) override;
    void Gui() override;

    void NewFramebuffer(int width, int height);
    void GenerateOceanVertex() const;
};
