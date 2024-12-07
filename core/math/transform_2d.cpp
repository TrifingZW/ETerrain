//
// Created by TrifingZW on 24-11-22.
//

#include "transform_2d.h"

using namespace glm;

// 构造函数，初始化默认值
Transform2D::Transform2D() : position(0.0f), rotation(0.0f), scale(1.0f) {
}

// Getters
vec2 Transform2D::GetPosition() const { return position; }
float Transform2D::GetRotation() const { return rotation; }
vec2 Transform2D::GetScale() const { return scale; }

// Setters
void Transform2D::SetPosition(const vec2 &pos) { position = pos; }
void Transform2D::SetRotation(const float rot) { rotation = rot; }
void Transform2D::SetScale(const vec2 &scl) { scale = scl; }

// 获取变换矩阵
mat3 Transform2D::GetMatrix() const {
    auto translationMatrix = mat3(1.0f);
    translationMatrix[2] = vec3(position, 1.0f);

    const auto rotationMatrix = mat3(
        vec3(cos(rotation), sin(rotation), 0.0f),
        vec3(-sin(rotation), cos(rotation), 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    );

    auto scaleMatrix = mat3(1.0f);
    scaleMatrix[0][0] = scale.x;
    scaleMatrix[1][1] = scale.y;

    return translationMatrix * rotationMatrix * scaleMatrix;
}

// 平移
void Transform2D::Translate(const vec2 &delta) {
    position += delta;
}

// 旋转
void Transform2D::Rotate(const float delta) {
    rotation += delta;
}

// 缩放
void Transform2D::ScaleBy(const vec2 &factor) {
    scale *= factor;
}
