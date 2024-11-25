//
// Created by TrifingZW on 24-11-22.
//

#pragma once
#include "node_3d.h"
#include "math/transform_3d.h"

enum class ProjectionMode {
    Perspective,
    Orthographic
};

class Camera3D : public Node3D {
    ProjectionMode _projectionMode = ProjectionMode::Perspective;
    float _aspectRatio = 16.0f / 9.0f;
    float _orthographicSize = 10.0f;
    float _fov = 45.0f, _near = 0.1f, _far = 100.0f;

public:
    [[nodiscard]] float GetFov() const;
    [[nodiscard]] float GetNear() const;
    [[nodiscard]] float GetFar() const;
    [[nodiscard]] float GetAspectRatio() const;
    [[nodiscard]] ProjectionMode GetProjectionMode() const;
    [[nodiscard]] float GetOrthographicSize() const;

    void SetFov(float fov);
    void SetNear(float near);
    void SetFar(float far);
    void SetAspectRatio(float aspectRatio);
    void SetProjectionMode(ProjectionMode mode);
    void SetOrthographicSize(float size);

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
};
