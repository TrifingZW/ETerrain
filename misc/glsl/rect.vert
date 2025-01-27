layout (location = 0) in vec4 vertex;
layout (location = 0) in vec4 color;

out vec4 Color;

uniform mat4 uTransform;

void main()
{
    Color = color;
    gl_Position = uTransform * vertex;
}