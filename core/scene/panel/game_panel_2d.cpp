//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>
#include <random>

#include "core.h"
#include "editor.h"
#include "glm/gtx/string_cast.hpp"

void GamePanel2D::Init()
{
    NewFramebuffer(width, height);

    // 顶点着色器
    constexpr auto vShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        layout (location = 1) in vec2 textureCoord;

        out vec2 TexCoords;

        uniform mat4 uTransform;

        void main()
        {
            TexCoords = textureCoord;
            gl_Position = uTransform * vertex;
        }
    )";
    // 片段着色器
    constexpr auto fShaderCode = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;

        uniform sampler2D image;
        uniform sampler2D colorUV;

        void main()
        {
            color = texture(image, TexCoords);
        }
    )";

    shader = new Shader(vShaderCode, fShaderCode, Shader::ShaderSourceType::Code);
}

void GamePanel2D::Ready()
{
    binParser.Parse("world.bin");
    hexManager = new HexManager(binParser.GetWidth(), binParser.GetHeight(), HexMetrics::TileWidth, HexMetrics::TileHeight);

    const auto data = new GLubyte[binParser.GetSize() * 4];
    for (size_t y = 0; y < binParser.GetHeight(); y++)
    {
        for (size_t x = 0; x < binParser.GetWidth(); x++)
        {
            const size_t index = (y * width + x) * 4;
            data[index] = 0; // 红色通道
            data[index + 1] = 0; // 绿色通道
            data[index + 2] = 0; // 蓝色通道
            data[index + 3] = binParser.topographies[y * binParser.GetWidth() + x].地块类型 == 1 ? 0 : 1; // alpha通道，完全不透明
        }
    }
    colorUV = new Texture2D();
    colorUV->Generate(binParser.GetHeight(), binParser.GetWidth());
    colorUV->SetData(data);
}

void GamePanel2D::Rendering(SpriteBatch& spriteBatch)
{
    ResourceTextureParser resource_texture_parser = Editor::loadResources->plantResourceTextureParser;

    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, SamplerState::LinearMirror);

    shader->Apply();
    shader->SetMatrix4("uTransform", camera2d->GetProjectionMatrix());
    shader->SetInt("image", 0);
    shader->SetInt("colorUV", 1);
    Core::GetGraphicsDevice()->textures[1] = colorUV;

    spriteBatch.Draw(
        Editor::loadResources->mapLand,
        Rect(0, 0, hexManager->GetPixelWidth(), hexManager->GetPixelHeight()),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    spriteBatch.End();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());

    for (size_t y = 0; y < binParser.GetHeight(); y++)
    {
        for (size_t x = 0; x < binParser.GetWidth(); x++)
        {
            glm::vec2 position = hexManager->HexToPixel(glm::ivec2(x, y));
            spriteBatch.DrawCenter(
                &resource_texture_parser.Texture2D,
                position,
                resource_texture_parser.GetRect("cactus_6.png"),
                Color(1.0f, 1.0f, 1.0f, 1.0f)
            );
        }
    }

    spriteBatch.End();

    Core::GetGraphicsDevice()->ResetRenderTarget();
}

void GamePanel2D::Gui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框

    ImGui::Begin("Game2D");

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


    static float fov = 1.0f;
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
    camera2d->SetZoom(fov);
    ImGui::Begin("Game Test");
    ImGui::Text("This is some useful text.");
    ImGui::SliderFloat("fov", &fov, 0.1f, 10.0f);
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

void GamePanel2D::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera2d->SetViewportSize(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(1.0f);

    if (renderTarget)
        renderTarget->Reinitialize(width, height);
    else
        renderTarget = new RenderTarget(width, height);
}
