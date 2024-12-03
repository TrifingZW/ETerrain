//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>

#include "core.h"
#include "editor.h"

void GamePanel2D::Init()
{
    NewFramebuffer(width, height);
}

void GamePanel2D::Ready() {}

void GamePanel2D::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();
    spriteBatch.Begin(SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());
    spriteBatch.Draw(
        &Editor::loadResources->terrainResourceTextureParser.Texture2D,
        Rect(0.0f, 0.0f, 493.0f, 755.0f),
        Rect(0.0f, 0.0f, 100.0f, 100.0f),
        Color(0, 1, 1, 1),
        0,
        glm::vec2(10, 10),
        SpriteEffects::None
    );
    spriteBatch.Draw(
        &Editor::loadResources->plantResourceTextureParser.Texture2D,
        Rect(0.0f, 0.0f, 493.0f, 755.0f),
        Rect(0.0f, 0.0f, 100.0f, 100.0f),
        Color(0, 1, 1, 1),
        0,
        glm::vec2(10, 10),
        SpriteEffects::None
    );
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
}

void GamePanel2D::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera2d->SetViewportSize(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(5.0f);

    if (renderTarget)
        renderTarget->Reinitialize(width, height);
    else
        renderTarget = new RenderTarget(width, height);
}
