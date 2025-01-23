//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include "node_2d.h"
#include "core/math/vector2.h"

class Camera2D : public Node2D
{
    float _zoom = 1.0f;
    Vector2 _offset = {0.0f, 0.0f};
    Vector2 _viewportSize = {0.0f, 0.0f};

public:
    [[nodiscard]] float GetZoom() const;
    [[nodiscard]] Vector2 GetOffset() const;
    [[nodiscard]] Vector2 GetViewportSize() const;

    void SetZoom(float zoom);
    void SetOffset(const Vector2& offset);
    void SetViewportSize(const Vector2& viewportSize);

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
};
