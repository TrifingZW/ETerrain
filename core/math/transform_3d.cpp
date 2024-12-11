//
// Created by TrifingZW on 24-11-22.
//

#include "transform_3d.h"

using namespace glm;

// Constructor
Transform3D::Transform3D(): _position(0.0f), _rotation(quat(1.0f, 0.0f, 0.0f, 0.0f)), _scale(1.0f) {}

// Getters
vec3 Transform3D::GetPosition() const { return _position; }
quat Transform3D::GetRotation() const { return _rotation; }
vec3 Transform3D::GetScale() const { return _scale; }

// Setters
void Transform3D::SetPosition(const vec3& pos) { _position = pos; }
void Transform3D::SetRotation(const quat& rot) { _rotation = rot; }

void Transform3D::SetRotationEuler(const vec3& eulerAngles)
{
    _rotation = quat(radians(eulerAngles));
}

void Transform3D::SetScale(const vec3& scl) { _scale = scl; }

// Utility methods
mat4 Transform3D::GetModelMatrix() const
{
    const mat4 translationMatrix = translate(mat4(1.0f), _position);
    const mat4 rotationMatrix = toMat4(_rotation);
    const mat4 scaleMatrix = glm::scale(mat4(1.0f), _scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

mat4 Transform3D::GetViewMatrix() const
{
    return lookAt(
        _position,
        // 摄像机位置
        _position + Forward(),
        // 目标位置
        UpAxis() // 上方向
    );
}

vec3 Transform3D::Forward() const
{
    return rotate(_rotation, vec3(0.0f, 0.0f, -1.0f));
}

vec3 Transform3D::UpAxis() const
{
    return rotate(_rotation, vec3(0.0f, 1.0f, 0.0f));
}

vec3 Transform3D::RightAxis() const
{
    return rotate(_rotation, vec3(1.0f, 0.0f, 0.0f));
}

void Transform3D::Translate(const vec3& delta)
{
    _position += delta;
}

void Transform3D::Rotate(const quat& delta)
{
    _rotation = normalize(delta * _rotation);
}

void Transform3D::RotateEuler(const vec3& eulerAngles)
{
    const auto delta = quat(radians(eulerAngles));
    Rotate(delta);
}

void Transform3D::ScaleBy(const vec3& factor)
{
    _scale *= factor;
}
