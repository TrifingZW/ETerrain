//
// Created by TrifingZW on 24-12-1.
//

#pragma once

namespace Graphics
{
    enum class SpriteEffects
    {
        /// <summary>
        /// No options specified.
        /// </summary>
        None = 0,
        /// <summary>
        /// Render the sprite reversed along the X axis.
        /// </summary>
        FlipHorizontally = 1,
        /// <summary>
        /// Render the sprite reversed along the Y axis.
        /// </summary>
        FlipVertically = 2
    };

    enum class SpriteSortMode
    {
        /// <summary>
        /// All sprites are drawing when <see cref="SpriteBatch.End"/> invokes, in order of draw call sequence. Depth is ignored.
        /// </summary>
        Deferred = 0,
        /// <summary>
        /// Each sprite is drawing at individual draw call, instead of <see cref="SpriteBatch.End"/>. Depth is ignored.
        /// </summary>
        Immediate = 1,
        /// <summary>
        /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by texture prior to drawing. Depth is ignored.
        /// </summary>
        Texture = 2,
        /// <summary>
        /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by depth in back-to-front order prior to drawing.
        /// </summary>
        BackToFront = 3,
        /// <summary>
        /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by depth in front-to-back order prior to drawing.
        /// </summary>
        FrontToBack = 4
    };

    enum class SetDataOptions
    {
        /// <summary>
        /// The SetData can overwrite the portions of existing data.
        /// </summary>
        None = 0,
        /// <summary>
        /// The SetData will discard the entire buffer. A pointer to a new memory area is returned and rendering from the previous area do not stall.
        /// </summary>
        Discard = 1,
        /// <summary>
        /// The SetData operation will not overwrite existing data. This allows the driver to return immediately from a SetData operation and continue rendering.
        /// </summary>
        NoOverwrite = 2
    };

    enum class TextureAddressMode
    {
        /// <summary>
        /// Texels outside range will form the tile at every integer junction.
        /// </summary>
        Wrap,
        /// <summary>
        /// Texels outside range will be setted to color of 0.0 or 1.0 texel.
        /// </summary>
        Clamp,
        /// <summary>
        /// Same as <see cref="TextureAddressMode.Wrap"/> but tiles will also flipped at every integer junction.
        /// </summary>
        Mirror,
    };

    enum class TextureFilter
    {
        /// <summary>
        /// Use linear filtering.
        /// </summary>
        Linear,
        /// <summary>
        /// Use point filtering.
        /// </summary>
        Point,
        /// <summary>
        /// Use anisotropic filtering.
        /// </summary>
        Anisotropic,
        /// <summary>
        /// Use linear filtering to shrink or expand, and point filtering between mipmap levels (mip).
        /// </summary>
        LinearMipPoint,
        /// <summary>
        /// Use point filtering to shrink (minify) or expand (magnify), and linear filtering between mipmap levels.
        /// </summary>
        PointMipLinear,
        /// <summary>
        /// Use linear filtering to shrink, point filtering to expand, and linear filtering between mipmap levels.
        /// </summary>
        MinLinearMagPointMipLinear,
        /// <summary>
        /// Use linear filtering to shrink, point filtering to expand, and point filtering between mipmap levels.
        /// </summary>
        MinLinearMagPointMipPoint,
        /// <summary>
        /// Use point filtering to shrink, linear filtering to expand, and linear filtering between mipmap levels.
        /// </summary>
        MinPointMagLinearMipLinear,
        /// <summary>
        /// Use point filtering to shrink, linear filtering to expand, and point filtering between mipmap levels.
        /// </summary>
        MinPointMagLinearMipPoint,
    };

    enum class VertexAttributeType
    {
        Float = 4
    };
}
