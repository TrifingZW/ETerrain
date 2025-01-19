//
// Created by TrifingZW on 24-11-22.
//

#include "transform_2d.h"

using namespace glm;

// 构造函数，初始化默认值
Transform2D::Transform2D() : Position(0.0f), Rotation(0.0f), Scale(1.0f) {}

// Getters
vec2 Transform2D::GetPosition() const { return Position; }
float Transform2D::GetRotation() const { return Rotation; }
vec2 Transform2D::GetScale() const { return Scale; }

// Setters
void Transform2D::SetPosition(const vec2& pos) { Position = pos; }
void Transform2D::SetRotation(const float rot) { Rotation = rot; }
void Transform2D::SetScale(const vec2& scl) { Scale = scl; }

// 获取变换矩阵
mat3 Transform2D::GetMatrix() const
{
    auto translationMatrix = mat3(1.0f);
    translationMatrix[2] = vec3(Position, 1.0f);

    const auto rotationMatrix = mat3(
        vec3(cos(Rotation), sin(Rotation), 0.0f),
        vec3(-sin(Rotation), cos(Rotation), 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    );

    auto scaleMatrix = mat3(1.0f);
    scaleMatrix[0][0] = Scale.x;
    scaleMatrix[1][1] = Scale.y;

    return translationMatrix * rotationMatrix * scaleMatrix;
}

// 平移
void Transform2D::Translate(const vec2& delta)
{
    Position += delta;
}

// 旋转
void Transform2D::Rotate(const float delta)
{
    Rotation += delta;
}

// 缩放
void Transform2D::ScaleBy(const vec2& factor)
{
    Scale *= factor;
}
