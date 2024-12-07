//
// Created by TrifingZW on 24-12-5.
//

#pragma once
#include "scene/3d/camera_3d.h"


class HexPanel : public Node
{
public:
    Camera3D* camera3d = nullptr;
    RenderTarget* renderTarget = nullptr;


    explicit HexPanel()
    {
        auto* camera = new Camera3D();
        camera3d = camera;
        AddChild(std::unique_ptr<Node>(camera));
    }

    void Init() override;
    void Ready() override;
    void Rendering(SpriteBatch& spriteBatch) override;
    void Gui() override;
    void Input(int key) override;

    GLuint VAO{}, VBO{}, EBO{};
    int width = 1, height = 1;
    Shader shader{};

    void NewFramebuffer(int width, int height);
};
