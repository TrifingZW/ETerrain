//
// Created by TrifingZW on 24-11-22.
//

#include "transform_3d.h"
#include <glm/gtx/quaternion.hpp>

using namespace glm;

// Constructor
Transform3D::Transform3D(): position(0.0f), rotation(quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f) {
}

// Getters
vec3 Transform3D::GetPosition() const { return position; }
quat Transform3D::GetRotation() const { return rotation; }
vec3 Transform3D::GetScale() const { return scale; }

// Setters
void Transform3D::SetPosition(const vec3 &pos) { position = pos; }
void Transform3D::SetRotation(const quat &rot) { rotation = rot; }

void Transform3D::SetRotationEuler(const vec3 &eulerAngles) {
    rotation = quat(radians(eulerAngles));
}

void Transform3D::SetScale(const vec3 &scl) { scale = scl; }

// Utility methods
mat4 Transform3D::GetMatrix() const {
    const mat4 translationMatrix = glm::translate(mat4(1.0f), position);
    const mat4 rotationMatrix = toMat4(rotation);
    const mat4 scaleMatrix = glm::scale(mat4(1.0f), scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

mat4 Transform3D::GetViewMatrix() const {
    return lookAt(
        position,
        // 摄像机位置
        position + Forward(),
        // 目标位置
        UpAxis() // 上方向
    );
}

vec3 Transform3D::Forward() const {
    return glm::rotate(rotation, vec3(0.0f, 0.0f, -1.0f));
}

vec3 Transform3D::UpAxis() const {
    return glm::rotate(rotation, vec3(0.0f, 1.0f, 0.0f));
}

vec3 Transform3D::RightAxis() const {
    return glm::rotate(rotation, vec3(1.0f, 0.0f, 0.0f));
}

void Transform3D::Translate(const vec3 &delta) {
    position += delta;
}

void Transform3D::Rotate(const quat &delta) {
    rotation = normalize(delta * rotation);
}

void Transform3D::RotateEuler(const vec3 &eulerAngles) {
    const auto delta = quat(radians(eulerAngles));
    Rotate(delta);
}

void Transform3D::ScaleBy(const vec3 &factor) {
    scale *= factor;
}
