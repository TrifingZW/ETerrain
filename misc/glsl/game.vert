layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 textureCoord;

out vec2 TexCoords;

uniform mat4 uTransform;

void main()
{
    TexCoords = textureCoord;
    gl_Position = uTransform * vertex;
}