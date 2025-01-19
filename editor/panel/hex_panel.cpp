//
// Created by TrifingZW on 24-12-5.
//

#include "hex_panel.h"

#include <imgui.h>

#include "core/core.h"
#include "hex_manager.h"

void HexPanel::Init()
{
    NewFramebuffer(width, height);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 绑定 VBO 并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HexMetrics::Corners), HexMetrics::Corners,GL_STATIC_DRAW);
    /*// 绑定 EBO 并上传数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);*/
    // 位置属性
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    Core::GetGraphicsDevice()->ResetBuffer();


    // 顶点着色器
    constexpr auto vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position =  projection * view * model * vec4(aPos, 1.0);
        }
    )";

    // 片段着色器
    constexpr auto* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";

    shader = Shader(vertexShaderSource, fragmentShaderSource, Shader::ShaderSourceType::Code);
}

void HexPanel::Ready() {}

void HexPanel::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

    constexpr auto model = glm::mat4(1.0f);
    shader.Apply();
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", camera3d->Transform3D.GetViewMatrix());
    shader.SetMatrix4("projection", camera3d->GetProjectionMatrix());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);


    Core::GetGraphicsDevice()->ResetRenderTarget();
}

void HexPanel::Gui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框

    ImGui::Begin("Hex");

    const ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (static_cast<int>(panelSize.x) != width || static_cast<int>(panelSize.y) != height)
    {
        width = static_cast<int>(panelSize.x);
        height = static_cast<int>(panelSize.y);
        NewFramebuffer(width, height);
    }

    ImGui::Image(
        static_cast<ImTextureID>(static_cast<intptr_t>(renderTarget->textureBuffer)),
        panelSize,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void HexPanel::Input(int key)
{
    if (ImGui::IsKeyDown(ImGuiKey_W))
        camera3d->Transform3D.Translate(normalize(camera3d->Transform3D.Forward()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_S))
        camera3d->Transform3D.Translate(-normalize(camera3d->Transform3D.Forward()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_A))
        camera3d->Transform3D.Translate(-normalize(camera3d->Transform3D.RightAxis()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_D))
        camera3d->Transform3D.Translate(normalize(camera3d->Transform3D.RightAxis()) * 0.01f);
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        camera3d->Transform3D.Translate(normalize(camera3d->Transform3D.UpAxis()) * 0.1f);
    if (ImGui::IsKeyDown(ImGuiKey_Q))
        camera3d->Transform3D.RotateEuler(glm::vec3(-0.1f, 0.0f, 0.0f));
}

void HexPanel::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera3d->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    if (renderTarget)
        renderTarget->Reinitialize(width, height, true);
    else
        renderTarget = new RenderTarget(width, height, true);
}
