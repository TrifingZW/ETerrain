//
// Created by TrifingZW on 24-12-5.
//

#include "hex_panel.h"

#include <imgui.h>

#include "core.h"

void HexPanel::Init()
{
    NewFramebuffer(width, height);
}

void HexPanel::Ready() {}

void HexPanel::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

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

void HexPanel::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera2d->SetViewportSize(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(1.0f);

    if (renderTarget)
        renderTarget->Reinitialize(width, height, true);
    else
        renderTarget = new RenderTarget(width, height, true);
}
