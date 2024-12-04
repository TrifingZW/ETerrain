//
// Created by TrifingZW on 24-12-5.
//

#pragma once
#include "scene/2d/camera_2d.h"


class HexPanel : public Node
{
public:
    Camera2D* camera2d = nullptr;
    RenderTarget* renderTarget = nullptr;


    explicit HexPanel()
    {
        auto* camera = new Camera2D();
        camera2d = camera;
        AddChild(std::unique_ptr<Node>(camera));
    }

    void Init() override;
    void Ready() override;
    void Rendering(SpriteBatch& spriteBatch) override;
    void Gui() override;

    int width = 1, height = 1;

    void NewFramebuffer(int width, int height);
};
