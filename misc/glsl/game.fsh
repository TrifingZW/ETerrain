#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D colorUV;

uniform float tileWidth;
uniform float tileHeight;

float cross(vec2 pCurrent, vec2 pOther)
{
    return pCurrent.x * pOther.y - pCurrent.y * pOther.x;
}

int mod(int p_x, int p_y)
{
    int value = p_x % p_y;
    if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0))
    {
        value += p_y;
    }
    return value;
}

ivec2 PixelToHex(vec2 pixel) {
    vec2 ret = pixel / vec2(tileWidth, tileHeight);

    const float overlapping_ratio = 0.75;
    ret.x /= overlapping_ratio;

    vec2 raw_pos = ret;
    if (mod(floor(ret.x), 2.0) > 0.5)
    ret = vec2(floor(ret.x), floor(ret.y + 0.5) - 0.5);
    else
    ret = floor(ret);

    vec2 in_tile_pos = raw_pos - ret;
    bool in_top_left_triangle = cross(in_tile_pos - vec2(0.0, 0.5), vec2(1.0 / overlapping_ratio - 1.0, -0.5)) > 0.0;
    bool in_bottom_left_triangle = cross(in_tile_pos - vec2(0.0, 0.5), vec2(1.0 / overlapping_ratio - 1.0, 0.5)) <= 0.0;

    ret = floor(ret);
    if (in_top_left_triangle)
    ret += ivec2(-1, mod(floor(ret.x), 2.0) > 0.5 ? 0 : -1);
    else if (in_bottom_left_triangle)
    ret += ivec2(-1, mod(floor(ret.x), 2.0) > 0.5 ? 1 : 0);

    return ivec2(ret);
}

void main()
{
/**  if (texture(colorUV, PixelToHex(TexCoords * 1024) / (TexCoords * 1024)).a == 0.0) {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    }*/
    color = texture(image, TexCoords);
}

