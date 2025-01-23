//
// Created by TrifingZW on 25-1-21.
//

#pragma once
#include <imgui.h>

#include "glm/vec2.hpp"
#include "glm/detail/func_packing_simd.inl"
#include "scene/2d/camera_2d.h"

namespace ImGuiHelper
{
    inline Vector2 GetMousePositionInItem()
    {
        const ImVec2 mouse_pos = ImGui::GetMousePos();
        const ImVec2 item_min = ImGui::GetItemRectMin();
        return {mouse_pos.x - item_min.x, mouse_pos.y - item_min.y};
    }

    inline Vector2 GetMousePositionInCamera2DWorld(const Camera2D& camera2d, const Vector2 localPos)
    {
        return (localPos - camera2d.GetViewportSize() / 2.0f) / camera2d.GetZoom() + camera2d.Transform2D.Position;
    }
}
