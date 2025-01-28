layout (location = 0) in vec4 vertex;

uniform mat4 uTransform;

void main()
{
    Color = color;
    gl_Position = uTransform * vertex;
}