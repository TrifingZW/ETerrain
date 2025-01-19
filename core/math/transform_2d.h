//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform2D
{
public:
    glm::vec2 Position; // 位置
    float Rotation; // 旋转角度（单位：弧度）
    glm::vec2 Scale; // 缩放比例

    // 构造函数
    Transform2D();

    // Getters
    [[nodiscard]] glm::vec2 GetPosition() const;
    [[nodiscard]] float GetRotation() const;
    [[nodiscard]] glm::vec2 GetScale() const;

    // Setters
    void SetPosition(const glm::vec2& pos);
    void SetRotation(float rot);
    void SetScale(const glm::vec2& scl);

    // 工具方法
    [[nodiscard]] glm::mat3 GetMatrix() const;
    void Translate(const glm::vec2& delta);
    void Rotate(float delta);
    void ScaleBy(const glm::vec2& factor);
};
