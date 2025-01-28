#ifdef GL_ES
precision mediump float;  // ES 必须声明浮点精度
#endif

out vec4 FragColor;

uniform vec4 Color;

void main()
{
    FragColor = Color;
}