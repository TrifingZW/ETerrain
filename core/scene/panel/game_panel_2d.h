//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <glad/glad.h>

#include "node.h"
#include "scene/2d/camera_2d.h"
#include "scene/resources/shader.h"

class GamePanel2D : public Node
{
public:
    Camera2D* camera2d = nullptr;
    RenderTarget* renderTarget = nullptr;

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

    int width = 1, height = 1;
    Shader shader{};
    glm::mat4 model{};

    void NewFramebuffer(int width, int height) ;
};
