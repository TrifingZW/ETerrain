//
// Created by TrifingZW on 24-11-22.
//

#pragma once
#include "vector2.h"

class Transform2D
{
public:
    Vector2 Position; // 位置
    float Rotation; // 旋转角度（单位：弧度）
    Vector2 Scale; // 缩放比例

    // 构造函数
    Transform2D();

    // Getters
    [[nodiscard]] Vector2 GetPosition() const;
    [[nodiscard]] float GetRotation() const;
    [[nodiscard]] Vector2 GetScale() const;

    // Setters
    void SetPosition(const Vector2& pos);
    void SetRotation(float rot);
    void SetScale(const Vector2& scl);

    // 工具方法
    [[nodiscard]] glm::mat3 GetMatrix() const;
    void Translate(const Vector2& delta);
    void Rotate(float delta);
    void ScaleBy(const Vector2& factor);
};
