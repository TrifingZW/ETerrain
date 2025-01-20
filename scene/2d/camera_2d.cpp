//
// Created by TrifingZW on 24-11-22.
//

#include "camera_2d.h"

using namespace glm;

float Camera2D::GetZoom() const { return _zoom; }

vec2 Camera2D::GetOffset() const { return _offset; }

void Camera2D::SetZoom(const float zoom) { _zoom = zoom; }

void Camera2D::SetOffset(const vec2& offset) { _offset = offset; }

void Camera2D::SetViewportSize(const vec2& viewportSize) { _viewportSize = viewportSize; }

mat4 Camera2D::GetProjectionMatrix() const
{
    // 计算视口的大小，考虑缩放和宽高比
    const float width = _viewportSize.x / _zoom;
    const float height = _viewportSize.y / _zoom;
    const float left = Transform2D.Position.x - width / 2.0f;
    const float right = Transform2D.Position.x + width / 2.0f;
    const float top = Transform2D.Position.y - height / 2.0f;
    const float bottom = Transform2D.Position.y + height / 2.0f;

    mat4 projection = ortho(left, right, bottom, top, -1.0f, 1.0f);
    projection = translate(projection, vec3(_offset, 0.0f));
    return projection;
}
