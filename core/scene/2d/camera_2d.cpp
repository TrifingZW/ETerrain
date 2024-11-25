//
// Created by TrifingZW on 24-11-22.
//

#include "camera_2d.h"

using namespace glm;

float Camera2D::GetZoom() const { return _zoom; }

vec2 Camera2D::GetOffset() const { return _offset; }

void Camera2D::SetZoom(const float zoom) { _zoom = zoom; }

void Camera2D::SetOffset(const vec2 &offset) { _offset = offset; }

void Camera2D::SetViewportSize(const vec2 &viewportSize) { _viewportSize = viewportSize; }

mat4 Camera2D::GetProjectionMatrix() const {
    // 计算视口的大小，考虑缩放和宽高比
    const float left = 0.0f;
    const float right = _viewportSize.x / _zoom; // 将右边界设为800除以缩放系数
    const float bottom = _viewportSize.y / _zoom; // 将下边界设为600除以缩放系数
    const float top = 0.0f;

    mat4 projection = ortho(left, right, bottom, top, -1.0f, 1.0f);
    projection = translate(projection, vec3(_offset, 0.0f));
    return projection;
}
