#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec2 texsize;

uniform sampler2D Image;
uniform sampler2D Color;

const float TileWidth = 148.0f;
const float TileHeight = 129.0f;
const float overlapping_ratio = 0.75f;

int PosMod(int p_x, int p_y)
{
    int value = p_x % p_y;
    if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0))
    {
        value += p_y;
    }
    return value;
}

float cross2D(vec2 a, vec2 b) {
    return a.x * b.y - a.y * b.x;
}

ivec2 getGridCoords(vec2 pixel) {
    vec2 ret = pixel;
    ret /= vec2(TileWidth * overlapping_ratio, TileHeight);

    const vec2 raw_pos = ret;
    if (PosMod(int(floor(ret.x)), 2) == 1) {
        ret = vec2(floor(ret.x), floor(ret.y + 0.5f) - 0.5f);
    } else {
        ret = vec2(floor(ret.x), floor(ret.y));
    }

    const vec2 in_tile_pos = raw_pos - ret;

    ivec2 retI = ivec2(ret);

    if (cross2D((in_tile_pos - vec2(0.0f, 0.5f)), (vec2(1.0f / overlapping_ratio - 1, -0.5f))) > 0) {
        int result = PosMod(int(floor(ret.x)), 2) != 0 ? 0 : -1;
        retI += ivec2(-1, result);
    }

    if (cross2D((in_tile_pos - vec2(0.0f, 0.5f)), (vec2(1.0f / overlapping_ratio - 1, 0.5f))) <= 0) {
        int result = PosMod(int(floor(ret.x)), 2) != 0 ? 1 : 0;
        retI += ivec2(-1, result);
    }

    return retI;
}

void main()
{
    vec4 currentColor = texelFetch(Color, getGridCoords(texsize * TexCoords), 0);
    if (currentColor.a == 0) {
        FragColor = texture(Image, TexCoords);
    }
    else {
        discard;
    }
}

