//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform3D {
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;

public:
    // Constructor
    Transform3D();

    // Getters
    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::quat GetRotation() const;
    [[nodiscard]] glm::vec3 GetScale() const;

    // Setters
    void SetPosition(const glm::vec3 &pos);
    void SetRotation(const glm::quat &rot);
    void SetRotationEuler(const glm::vec3 &eulerAngles);
    void SetScale(const glm::vec3 &scl);

    // Utility methods
    [[nodiscard]] glm::mat4 GetModelMatrix() const;
    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    [[nodiscard]] glm::vec3 Forward() const;
    [[nodiscard]] glm::vec3 UpAxis() const;
    [[nodiscard]] glm::vec3 RightAxis() const;

    void Translate(const glm::vec3 &delta);
    void Rotate(const glm::quat &delta);
    void RotateEuler(const glm::vec3 &eulerAngles);
    void ScaleBy(const glm::vec3 &factor);
};
