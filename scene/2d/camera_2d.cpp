//
// Created by TrifingZW on 24-11-22.
//

#include "camera_2d.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

float Camera2D::GetZoom() const { return _zoom; }

Vector2 Camera2D::GetOffset() const { return _offset; }

Vector2 Camera2D::GetViewportSize() const { return _viewportSize; }

void Camera2D::SetZoom(const float zoom) { _zoom = zoom; }

void Camera2D::SetOffset(const Vector2& offset) { _offset = offset; }

void Camera2D::SetViewportSize(const Vector2& viewportSize) { _viewportSize = viewportSize; }

glm::mat4 Camera2D::GetProjectionMatrix() const
{
    // 计算视口的大小，考虑缩放和宽高比
    const float width = _viewportSize.X / _zoom;
    const float height = _viewportSize.Y / _zoom;
    const float left = Transform2D.Position.X - width / 2.0f;
    const float right = Transform2D.Position.X + width / 2.0f;
    const float top = Transform2D.Position.Y - height / 2.0f;
    const float bottom = Transform2D.Position.Y + height / 2.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    projection = translate(projection, glm::vec3(static_cast<glm::vec2>(_offset), 0.0f));
    return projection;
}
