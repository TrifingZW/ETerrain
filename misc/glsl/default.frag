#ifdef GL_ES
precision mediump float;  // ES 必须声明浮点精度
#endif

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Image;

void main()
{
    FragColor = texture(Image, TexCoords);
}