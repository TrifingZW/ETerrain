//
// Created by TrifingZW on 24-11-18.
//

#include "game_panel_3d.h"

#include <imgui.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

#include "core/core.h"
#include "editor/editor.h"

using namespace glm;

void GamePanel3D::Init()
{
    NewFramebuffer();
    InitOpenGlResources();
}

void GamePanel3D::Ready()
{
    texture1 = Editor::loadResources->plantResourceTextureParser.Texture2D.Id;
    texture2 = Editor::loadResources->terrainResourceTextureParser.Texture2D.Id;
}

constexpr vec3 cubePositions[] = {
    vec3(0.0f, 0.0f, 0.0f),
    vec3(2.0f, 5.0f, -15.0f),
    vec3(-1.5f, -2.2f, -2.5f),
    vec3(-3.8f, -2.0f, -12.3f),
    vec3(2.4f, -0.4f, -3.5f),
    vec3(-1.7f, 3.0f, -7.5f),
    vec3(1.3f, -2.0f, -2.5f),
    vec3(1.5f, 2.0f, -2.5f),
    vec3(1.5f, 0.2f, -1.5f),
    vec3(-1.3f, 1.0f, -1.5f)
};

void GamePanel3D::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();
    // glEnable(GL_DEPTH_TEST);

    mat4 transform = rotate(translate(mat4(1.0f), vec3(0.5f, -0.5f, 0.0f)), static_cast<float>(glfwGetTime()), vec3(0.0f, 1.0f, 1.0f));
    shader.Apply();
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);
    const GLint transformLoc = glGetUniformLocation(shader.shaderId, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    shader2.Apply();
    glBindVertexArray(VAO2);
    for (unsigned int i = 0; i < 10; i++)
    {
        model = mat4(1.0f);
        model = translate(model, cubePositions[i]);
        model = rotate(model,/*static_cast<float>(glfwGetTime()) **/radians(50.0f), vec3(1.0f, 0.3f, 0.5f));
        const float angle = 20.0f * static_cast<float>(i);
        model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
        shader2.SetInt("texture1", 0);
        shader2.SetInt("texture2", 1);
        shader2.SetMatrix4("model", model);
        shader2.SetMatrix4("view", camera3d->Transform3D.GetViewMatrix());
        shader2.SetMatrix4("projection", camera3d->GetProjectionMatrix());
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    Core::GetGraphicsDevice()->ResetBuffer();
    Core::GetGraphicsDevice()->ResetRenderTarget();
}

void GamePanel3D::Gui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框


    ImGui::Begin("Game3D");

    const ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (static_cast<int>(panelSize.x) != width || static_cast<int>(panelSize.y) != height)
    {
        width = static_cast<int>(panelSize.x);
        height = static_cast<int>(panelSize.y);
        NewFramebuffer();
    }

    ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(renderTarget->textureBuffer)), panelSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
    ImGui::PopStyleVar(2);

    static float fov = 45.0f;
    static float f1 = 0.0f;
    static float f2 = 0.0f;
    static float f3 = 0.0f;
    static float f4 = 0.0f;
    static float f5 = 0.0f;
    static float f6 = 0.0f;
    static float f7 = 0.0f;
    static float f8 = 0.0f;
    static float f9 = 0.0f;
    static auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    camera3d->SetFov(fov);
    ImGui::Begin("Game Test");
    ImGui::Text("This is some useful text.");
    ImGui::SliderFloat("fov", &fov, 0.0f, 179.0f);
    ImGui::SliderFloat("f1", &f1, -10.0f, 10.0f);
    ImGui::SliderFloat("f2", &f2, -10.0f, 10.0f);
    ImGui::SliderFloat("f3", &f3, -10.0f, 10.0f);
    ImGui::SliderFloat("f4", &f4, -10.0f, 10.0f);
    ImGui::SliderFloat("f5", &f5, -10.0f, 10.0f);
    ImGui::SliderFloat("f6", &f6, -10.0f, 10.0f);
    ImGui::SliderFloat("f7", &f7, -10.0f, 10.0f);
    ImGui::SliderFloat("f8", &f8, -10.0f, 10.0f);
    ImGui::SliderFloat("f9", &f9, -10.0f, 10.0f);
    ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void GamePanel3D::Input(int key)
{
    if (ImGui::IsKeyDown(ImGuiKey_W))
        camera3d->Transform3D.Translate(normalize(camera3d->Transform3D.Forward()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_S))
        camera3d->Transform3D.Translate(-normalize(camera3d->Transform3D.Forward()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_A))
        camera3d->Transform3D.Translate(-normalize(camera3d->Transform3D.RightAxis()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_D))
        camera3d->Transform3D.Translate(normalize(camera3d->Transform3D.RightAxis()) * 0.01f);
}

void GamePanel3D::NewFramebuffer()
{
    // 设置相机视口大小
    camera3d->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    if (renderTarget)
        renderTarget->Reinitialize(width, height, true);
    else
        renderTarget = new RenderTarget(width, height, true);
}

void GamePanel3D::InitOpenGlResources()
{
  // @formatter:off
  constexpr float vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };
  constexpr float vertices2[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  constexpr unsigned int indices[] = {
      // 注意索引从0开始!
      // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
      // 这样可以由下标代表顶点组合成矩形

      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };
    // @formatter:on

    // 创建 VAO VBO EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 绑定 VBO 并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
    // 绑定 EBO 并上传数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 创建 VAO2 VBO2 EBO2
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);
    // 绑定 VAO2
    glBindVertexArray(VAO2);
    // 绑定 VBO2 并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 切换回屏幕
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // 顶点着色器
    constexpr auto vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        uniform mat4 transform;

        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
    )";

    // 顶点着色器2
    constexpr auto vertexShaderSource2 = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    // 片段着色器
    constexpr auto* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main() {
            FragColor = mix(texture(texture1, TexCoord) * vec4(ourColor, 1.0), texture(texture2, TexCoord), 0.2);
        }
    )";

    // 片段着色器2
    constexpr auto* fragmentShaderSource2 = R"(
        #version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main() {
            FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
        }
    )";

    shader = Shader(vertexShaderSource, fragmentShaderSource, Shader::ShaderSourceType::Code);
    shader2 = Shader(vertexShaderSource2, fragmentShaderSource2, Shader::ShaderSourceType::Code);
}
