#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec2 textureCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 uTransform;

void main()
{
    TexCoords = textureCoord;
    gl_Position = uTransform * model * vec4(position + offset, 1.0f);
}