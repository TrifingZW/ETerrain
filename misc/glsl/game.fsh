#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D colorUV;

void main()
{
    color = vec4(0.2,0.2,0.5,1.0);
}