//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>
#include <random>
#include <glm/gtx/string_cast.hpp>

#include "core/core.h"
#include "game_shader.h"

GamePanel2D::GamePanel2D()
{
    auto* camera = new Camera2D();
    camera2d = camera;
    AddChild(std::unique_ptr<Node>(camera));
}

GamePanel2D::~GamePanel2D()
{
    delete shader;
    delete vertexInfo;
    delete hexManager;
    delete camera2d;
    delete renderTarget;
}

void GamePanel2D::Init()
{
    NewFramebuffer(width, height);
    shader = new Shader(GLSL::game_vsh, GLSL::game_fsh, Shader::ShaderSourceType::Code);
}

void GamePanel2D::Ready()
{
    binParser.Parse("world.bin");
    hexManager = new HexManager(binParser.GetWidth(), binParser.GetHeight(), 74.0f);

    vertexInfo = new HexVertexType();
    GenerateOceanVertex();
}

void GamePanel2D::Rendering(SpriteBatch& spriteBatch)
{
    ResourceTextureParser& resource_texture_parser = Editor::loadResources->plantResourceTextureParser;

    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, SamplerState::LinearMirror);

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

    shader->Apply();
    shader->SetMatrix4("uTransform", camera2d->GetProjectionMatrix());
    Core::GetGraphicsDevice()->textures[0] = Editor::loadResources->mapLand;
    Core::GetGraphicsDevice()->DrawUserPrimitives(GL_TRIANGLE_FAN, vertexInfo, 0, vertexInfo->PHexVertexData.size());

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

void GamePanel2D::GenerateOceanVertex() const
{
    for (size_t y = 0; y < binParser.GetHeight(); y++)
    {
        for (size_t x = 0; x < binParser.GetWidth(); x++)
        {
            if (binParser.topographies[y * binParser.GetWidth() + x].地块类型 != 1)
                continue;
            const size_t index = (y * binParser.GetWidth() + x) * 6;
            const glm::vec2 position = hexManager->HexToPixel({x, y});
            glm::mat4 model(1.0f);
            model = translate(
                model,
                glm::vec3(
                    position.x,
                    position.y,
                    0.0f
                )
            );
            model = scale(model, glm::vec3(glm::vec2(HexMetrics::TileWidth / 2.0f), 1.0f));
            for (auto Corner: HexMetrics::Corners)
                vertexInfo->PHexVertexData.push_back(
                    {
                        model * glm::vec4(Corner, 1.0f)
                    }
                );
        }
    }
}

