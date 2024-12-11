//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include "node_2d.h"

class Camera2D : public Node2D
{
    float _zoom = 1.0f;
    glm::vec2 _offset = {0.0f, 0.0f};
    glm::vec2 _viewportSize = {0.0f, 0.0f};

public:
    [[nodiscard]] float GetZoom() const;
    [[nodiscard]] glm::vec2 GetOffset() const;

    void SetZoom(float zoom);
    void SetOffset(const glm::vec2& offset);
    void SetViewportSize(const glm::vec2& viewportSize);

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
};
