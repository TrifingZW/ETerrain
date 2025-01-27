#ifdef GL_ES
precision mediump float;  // ES 必须声明浮点精度
#endif

in vec4 Color;
out vec4 FragColor;

void main()
{
    FragColor = Color;
}