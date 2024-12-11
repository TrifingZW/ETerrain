//
// Created by TrifingZW on 24-11-22.
//

#include "camera_3d.h"

using namespace glm;

float Camera3D::GetFov() const { return _fov; }

float Camera3D::GetNear() const { return _near; }

float Camera3D::GetFar() const { return _far; }

float Camera3D::GetAspectRatio() const { return _aspectRatio; }

ProjectionMode Camera3D::GetProjectionMode() const { return _projectionMode; }

float Camera3D::GetOrthographicSize() const { return _orthographicSize; }

void Camera3D::SetFov(const float fov)
{
    if (fov <= 0.0f || fov >= 180.0f)
    {
        throw std::invalid_argument("Field of view must be between 0 and 180 degrees.");
    }
    _fov = fov;
}

void Camera3D::SetNear(const float near)
{
    if (near <= 0.0f)
    {
        throw std::invalid_argument("Near plane must be greater than 0.");
    }
    _near = near;
}

void Camera3D::SetFar(const float far)
{
    if (far <= _near)
    {
        throw std::invalid_argument("Far plane must be greater than near plane.");
    }
    _far = far;
}

void Camera3D::SetProjectionMode(const ProjectionMode mode) { _projectionMode = mode; }

void Camera3D::SetOrthographicSize(const float size)
{
    if (size <= 0.0f)
    {
        throw std::invalid_argument("Orthographic size must be greater than 0.");
    }
    _orthographicSize = size;
}

void Camera3D::SetAspectRatio(const float aspectRatio)
{
    if (aspectRatio <= 0.0f)
    {
        throw std::invalid_argument("Aspect ratio must be greater than 0.");
    }
    _aspectRatio = aspectRatio;
}

mat4 Camera3D::GetProjectionMatrix() const
{
    if (_projectionMode == ProjectionMode::Perspective)
    {
        return perspective(radians(_fov), _aspectRatio, _near, _far);
    }

    const float halfSize = _orthographicSize / 2.0f;
    return ortho(-halfSize * _aspectRatio, halfSize * _aspectRatio, -halfSize, halfSize, _near, _far);
}
