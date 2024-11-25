//
// Created by TrifingZW on 24-11-18.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "node.h"
#include "scene/3d/camera_3d.h"
#include "scene/resources/shader.h"

class GamePanel3D : public Node {
public:
    Camera3D *camera3d = nullptr;

    explicit GamePanel3D() {
        auto *camera = new Camera3D();
        camera3d = camera;
        AddChild(std::unique_ptr<Node>(camera));
    }

    void Init() override;
    void Ready() override;
    void Rendering(const SpriteBatch &spriteBatch) override;
    void Gui() override;
    void Input(int key) override;

private:
    GLuint VAO{}, VBO{}, EBO{};
    GLuint VAO2{}, VBO2{}, EBO2{};
    GLuint framebuffer = {}, depthRenderbuffer{}, textureBuffer{};
    int width = 1, height = 1;
    Shader shader{}, shader2{};
    GLuint texture1{}, texture2{};
    glm::mat4 model{};
    void NewFramebuffer();
    void InitOpenGlResources();
};
