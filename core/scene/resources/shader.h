//
// Created by TrifingZW on 24-11-20.
//

#pragma once
#include <string>
#include <glad/glad.h>

#include "glm/glm.hpp"

class Shader {
public:
    enum class ShaderSourceType {
        Path,
        Code
    };

    // 程序ID
    GLuint shaderId;

    // 构造器读取并构建着色器
    Shader();
    Shader(const char *source1, const char *source2, ShaderSourceType type);
    // 使用/激活程序
    void Apply() const;
    // uniform工具函数
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetMatrix4(const std::string &name, glm::mat4 value) const;
    void SetVector3(const std::string &name, glm::vec3 value) const;

    static GLuint LinkShader(const char *vShaderCode, const char *fShaderCode);
    static GLuint CreateShader(const char *source, GLenum shader_type);
};
