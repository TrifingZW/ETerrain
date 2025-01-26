//
// Created by TrifingZW on 24-11-20.
//

#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.inl>

#include "core/core.h"
#include "default_shader.h"

Shader::Shader()
{
    shaderId = LinkShader(GLSL::default_vsh, GLSL::default_fsh);
}

Shader::Shader(const char* source1, const char* source2, ShaderSourceType type)
{
    if (type == ShaderSourceType::Path)
    {
        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // 打开文件
            vShaderFile.open(source1);
            fShaderFile.open(source2);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (const std::ifstream::failure& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: "
                    << e.what() << " (vertexPath: " << source1
                    << ", fragmentPath: " << source2 << ")" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        shaderId = LinkShader(vShaderCode, fShaderCode);
    }
    else if (type == ShaderSourceType::Code)
    {
        shaderId = LinkShader(source1, source2);
    }
}

Shader::~Shader()
{
    glDeleteProgram(shaderId);
}

void Shader::Apply() const
{
    Core::GetGraphicsDevice()->currentShaderId = shaderId;
    glUseProgram(shaderId);
}

void Shader::SetBool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::SetMatrix4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void Shader::SetVector2(const std::string& name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(shaderId, name.c_str()), 1, value_ptr(value));
}

void Shader::SetVector3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, value_ptr(value));
}

GLuint Shader::LinkShader(const char* vShaderCode, const char* fShaderCode)
{
    // 创建着色器程序
    const GLuint vertexShader = CreateShader(vShaderCode, GL_VERTEX_SHADER);
    const GLuint fragmentShader = CreateShader(fShaderCode, GL_FRAGMENT_SHADER);

    const GLuint id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return id;
}

GLuint Shader::CreateShader(const char* source, const GLenum shader_type)
{
    const GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}
