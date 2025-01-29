#ifdef GL_ES
precision mediump float;  // ES 必须声明全局浮点精度
#endif

in vec2 TexCoords;
out vec4 FragColor;

uniform float tileWidth;
uniform float tileHeight;
uniform vec2 texSize;

uniform sampler2D Image;
uniform sampler2D Color;

// 移除浮点字面量的 'f' 后缀（GLSL 不强制要求）
const float overlapping_ratio = 0.75;

// 整数取模函数（兼容 ES 3.20）
int PosMod(int p_x, int p_y) {
    int value = p_x % p_y;
    if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0)) {
        value += p_y;
    }
    return value;
}

// 二维叉乘计算
float cross(vec2 a, vec2 b) {
    return a.x * b.y - a.y * b.x;
}

// 计算网格坐标
ivec2 getGridCoords(vec2 pixel) {
    vec2 ret = pixel;
    ret /= vec2(tileWidth * overlapping_ratio, tileHeight);

    vec2 raw_pos = ret;
    if (PosMod(int(floor(ret.x)), 2) == 1) {
        ret = vec2(floor(ret.x), floor(ret.y + 0.5) - 0.5);
    } else {
        ret = vec2(floor(ret.x), floor(ret.y));
    }

    vec2 in_tile_pos = raw_pos - ret;

    ivec2 retI = ivec2(floor(ret));

    if (cross((in_tile_pos - vec2(0.0, 0.5)), (vec2(1.0 / overlapping_ratio - 1.0, -0.5))) > 0.0) {
        int result = PosMod(int(floor(ret.x)), 2) != 0 ? 0 : -1;
        retI += ivec2(-1, result);
    }

    if (cross((in_tile_pos - vec2(0.0, 0.5)), (vec2(1.0 / overlapping_ratio - 1.0, 0.5))) <= 0.0) {
        int result = PosMod(int(floor(ret.x)), 2) != 0 ? 1 : 0;
        retI += ivec2(-1, result);
    }

    return retI;
}

void main() {
    // 使用 texelFetch 和 texture 函数（ES 3.20 支持）
    vec4 currentColor = texelFetch(Color, getGridCoords(texSize * TexCoords), 0);
    if (currentColor.a == 0.0) {
        FragColor = texture(Image, TexCoords);
    } else {
        discard;
    }
}