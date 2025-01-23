//
// Created by TrifingZW on 24-11-22.
//

#include "transform_2d.h"


// 构造函数，初始化默认值
Transform2D::Transform2D() : Position(0.0f), Rotation(0.0f), Scale(1.0f) {}

// Getters
Vector2 Transform2D::GetPosition() const { return Position; }
float Transform2D::GetRotation() const { return Rotation; }
Vector2 Transform2D::GetScale() const { return Scale; }

// Setters
void Transform2D::SetPosition(const Vector2& pos) { Position = pos; }
void Transform2D::SetRotation(const float rot) { Rotation = rot; }
void Transform2D::SetScale(const Vector2& scl) { Scale = scl; }

// 获取变换矩阵
glm::mat3 Transform2D::GetMatrix() const
{
    auto translationMatrix = glm::mat3(1.0f);
    translationMatrix[2] = glm::vec3(static_cast<glm::vec2>(Position), 1.0f);

    const auto rotationMatrix = glm::mat3(
        glm::vec3(cos(Rotation), sin(Rotation), 0.0f),
        glm::vec3(-sin(Rotation), cos(Rotation), 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    auto scaleMatrix = glm::mat3(1.0f);
    scaleMatrix[0][0] = Scale.x;
    scaleMatrix[1][1] = Scale.y;

    return translationMatrix * rotationMatrix * scaleMatrix;
}

// 平移
void Transform2D::Translate(const Vector2& delta)
{
    Position += delta;
}

// 旋转
void Transform2D::Rotate(const float delta)
{
    Rotation += delta;
}

// 缩放
void Transform2D::ScaleBy(const Vector2& factor)
{
    Scale *= factor;
}
