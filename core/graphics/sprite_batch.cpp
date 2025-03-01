//
// Created by TrifingZW on 24-11-30.
//

#include "sprite_batch.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "core/math/math_funcs.h"

using namespace Graphics;

SpriteBatch::SpriteBatch(GraphicsDevice* graphicsDevice): _graphicsDevice(graphicsDevice)
{
    GenerateIndexArray(_indices);
    _bufferManager.SetIndexPointerEXT(nullptr, _indices, MAX_INDICES_SIZE);
}

SpriteBatch::~SpriteBatch() = default;

void SpriteBatch::Begin()
{
    Begin(SpriteSortMode::Deferred, SamplerState(), glm::mat4(1.0f));
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode)
{
    Begin(spriteSortMode, SamplerState(), glm::mat4(1.0f));
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, const glm::mat4& matrix)
{
    Begin(spriteSortMode, SamplerState(), matrix);
}

void SpriteBatch::Begin(const SamplerState& samplerState, const glm::mat4& matrix)
{
    Begin(SpriteSortMode::Deferred, samplerState, matrix);
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, const SamplerState& samplerState)
{
    Begin(spriteSortMode, samplerState, glm::mat4(1.0f));
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, SamplerState samplerState, const glm::mat4& matrix)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = spriteSortMode;
    _samplerState = std::move(samplerState);
    _matrix = matrix;

    if (_spriteSortMode == SpriteSortMode::Immediate)
        PrepRenderState();
}

void SpriteBatch::Draw(Texture2D* texture, const glm::vec2 position, const Color color)
{
    CheckBegin("Draw");
    PushSprite(
        texture,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        position.x,
        position.y,
        static_cast<float>(texture->Width),
        static_cast<float>(texture->Height),
        color,
        0.0f,
        0.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::DrawCenter(Texture2D* texture, const glm::vec2 position, const Color color)
{
    CheckBegin("Draw");
    PushSprite(
        texture,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        position.x,
        position.y,
        static_cast<float>(texture->Width),
        static_cast<float>(texture->Height),
        color,
        static_cast<float>(texture->Width) / 2.0f,
        static_cast<float>(texture->Height) / 2.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::Draw(Texture2D* texture, const glm::vec2 position, const Rect2 sourceRect, const Color color)
{
    const float sourceX = sourceRect.X / static_cast<float>(texture->Width);
    const float sourceY = sourceRect.Y / static_cast<float>(texture->Height);
    const float sourceW = sourceRect.Width / static_cast<float>(texture->Width);
    const float sourceH = sourceRect.Height / static_cast<float>(texture->Height);
    const float destW = sourceRect.Width;
    const float destH = sourceRect.Height;
    CheckBegin("Draw");
    PushSprite(
        texture,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        position.x,
        position.y,
        destW,
        destH,
        color,
        0.0f,
        0.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::DrawCenter(Texture2D* texture, const glm::vec2 position, const Rect2 sourceRect, const Color color)
{
    const float sourceX = sourceRect.X / static_cast<float>(texture->Width);
    const float sourceY = sourceRect.Y / static_cast<float>(texture->Height);
    const float sourceW = sourceRect.Width / static_cast<float>(texture->Width);
    const float sourceH = sourceRect.Height / static_cast<float>(texture->Height);
    const float destW = sourceRect.Width;
    const float destH = sourceRect.Height;
    CheckBegin("Draw");
    PushSprite(
        texture,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        position.x,
        position.y,
        destW,
        destH,
        color,
        static_cast<float>(sourceRect.Width) / 2.0f,
        static_cast<float>(sourceRect.Height) / 2.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::Draw(
    Texture2D* texture,
    const glm::vec2 position,
    const Rect2 sourceRect,
    const Color color,
    const float rotation,
    const glm::vec2 origin,
    const float scale,
    const SpriteEffects effects
)
{
    CheckBegin("Draw");
    float destW = scale;
    float destH = scale;
    const float sourceX = sourceRect.X / static_cast<float>(texture->Width);
    const float sourceY = sourceRect.Y / static_cast<float>(texture->Height);
    const float sourceW = glm::sign(sourceRect.Width)
                          * glm::max(glm::abs(sourceRect.Width), Math::Epsilon) / static_cast<float>(texture->Width);
    const float sourceH = glm::sign(sourceRect.Height)
                          * glm::max(glm::abs(sourceRect.Height), Math::Epsilon) / static_cast<float>(texture->Height);
    destW *= sourceRect.Width;
    destH *= sourceRect.Height;

    PushSprite(
        texture,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        position.x,
        position.y,
        destW,
        destH,
        color,
        origin.x,
        origin.y,
        rotation,
        effects
    );
}

void SpriteBatch::Draw(
    Texture2D* texture,
    const glm::vec2 position,
    const Rect2 sourceRect,
    const Color color,
    const float rotation,
    const glm::vec2 origin,
    glm::vec2 scale,
    const SpriteEffects effects
)
{
    CheckBegin("Draw");
    const float sourceX = sourceRect.X / static_cast<float>(texture->Width);
    const float sourceY = sourceRect.Y / static_cast<float>(texture->Height);
    const float sourceW = glm::sign(sourceRect.Width)
                          * glm::max(glm::abs(sourceRect.Width), Math::Epsilon) / static_cast<float>(texture->Width);
    const float sourceH = glm::sign(sourceRect.Height)
                          * glm::max(glm::abs(sourceRect.Height), Math::Epsilon) / static_cast<float>(texture->Height);
    scale.x *= sourceRect.Width;
    scale.y *= sourceRect.Height;

    PushSprite(
        texture,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        position.x,
        position.y,
        scale.x,
        scale.y,
        color,
        origin.x,
        origin.y,
        rotation,
        effects
    );
}

void SpriteBatch::Draw(Texture2D* texture, const Rect2 TargetRect, const Color color)
{
    CheckBegin("Draw");
    PushSprite(
        texture,
        0.0f,
        0.0f,
        TargetRect.Width / static_cast<float>(texture->Width),
        TargetRect.Height / static_cast<float>(texture->Height),
        TargetRect.X,
        TargetRect.Y,
        TargetRect.Width,
        TargetRect.Height,
        color,
        0.0f,
        0.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::Draw(Texture2D* texture, const Rect2 SourceRect, const Rect2 TargetRect, const Color color)
{
    CheckBegin("Draw");
    const float sourceX = SourceRect.X / static_cast<float>(texture->Width);
    const float sourceY = SourceRect.Y / static_cast<float>(texture->Height);
    const float sourceW = SourceRect.Width / static_cast<float>(texture->Width);
    const float sourceH = SourceRect.Height / static_cast<float>(texture->Height);

    PushSprite(
        texture,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        TargetRect.X,
        TargetRect.Y,
        TargetRect.Width,
        TargetRect.Height,
        color,
        0.0f,
        0.0f,
        0.0f,
        SpriteEffects::None
    );
}

void SpriteBatch::Draw(
    Texture2D* texture2D,
    const Rect2 SourceRect,
    const Rect2 TargetRect,
    const Color color,
    const float rotation,
    const glm::vec2 origin,
    const SpriteEffects effects
)
{
    CheckBegin("Draw");
    const float sourceX = SourceRect.X / static_cast<float>(texture2D->Width);
    const float sourceY = SourceRect.Y / static_cast<float>(texture2D->Height);
    const float sourceW = glm::sign(SourceRect.Width) * glm::max(
                              glm::abs(SourceRect.Width),
                              Math::Epsilon
                          ) / static_cast<float>(texture2D->Width);
    const float sourceH = glm::sign(SourceRect.Height) * glm::max(
                              glm::abs(SourceRect.Height),
                              Math::Epsilon
                          ) / static_cast<float>(texture2D->Height);

    PushSprite(
        texture2D,
        sourceX,
        sourceY,
        sourceW,
        sourceH,
        TargetRect.X,
        TargetRect.Y,
        TargetRect.Width,
        TargetRect.Height,
        color,
        origin.x,
        origin.y,
        rotation,
        effects
    );
}

void SpriteBatch::DrawRect(Texture2D texture, Rect2 rect, Color color) {}

void SpriteBatch::End()
{
    if (!_beginCalled)
        throw std::runtime_error("在成功调用 Begin 之前，无法再次调用 End。");
    _beginCalled = false;

    if (_spriteSortMode != SpriteSortMode::Immediate)
        FlushBatch();

    Clear();
}

void SpriteBatch::Clear()
{
    _spriteSortMode = SpriteSortMode::Deferred;
    _samplerState = SamplerState();
    _matrix = glm::mat4(1.0f);
    _beginCalled = false;
    _numSprites = 0;
}

void SpriteBatch::CheckBegin(const std::string& method) const
{
    if (!_beginCalled)
        throw std::runtime_error(
            method + " was called, but Begin has" +
            " not yet been called. Begin must be" +
            " called successfully before you can" +
            " call " + method + "."
        );
}

void SpriteBatch::PushSprite(
    Texture2D* texture,
    const float sourceX,
    const float sourceY,
    const float sourceW,
    const float sourceH,
    const float destinationX,
    const float destinationY,
    const float destinationW,
    const float destinationH,
    const Color color,
    const float originX,
    const float originY,
    const float rotation,
    const SpriteEffects effects
)
{
    if (_numSprites >= MAX_SPRITES)
        FlushBatch();

    if (_spriteSortMode == SpriteSortMode::Deferred)
    {
        GenerateVertexInfo(
            &_vertexInfo[_numSprites],
            sourceX,
            sourceY,
            sourceW,
            sourceH,
            destinationX,
            destinationY,
            destinationW,
            destinationH,
            color,
            originX,
            originY,
            rotation,
            effects
        );
        _textureInfo[_numSprites] = texture;
        _numSprites++;
    }
    else if (_spriteSortMode == SpriteSortMode::Immediate)
    {
        GenerateVertexInfo(
            &_vertexInfo[0],
            sourceX,
            sourceY,
            sourceW,
            sourceH,
            destinationX,
            destinationY,
            destinationW,
            destinationH,
            color,
            originX,
            originY,
            rotation,
            effects
        );
        const int offset = UpdateVertexBuffer(0, 1);
        DrawPrimitivesBase(texture, offset, 1);
    }
    else if (_spriteSortMode == SpriteSortMode::Texture)
    {
        SpriteInfo* sprite_info = &_spriteInfos[_numSprites];
        sprite_info->TextureID = texture->Id;
        sprite_info->SourceX = sourceX;
        sprite_info->SourceY = sourceY;
        sprite_info->SourceW = sourceW;
        sprite_info->SourceH = sourceH;
        sprite_info->DestinationX = destinationX;
        sprite_info->DestinationY = destinationY;
        sprite_info->DestinationW = destinationW;
        sprite_info->DestinationH = destinationH;
        sprite_info->Color = color;
        sprite_info->OriginX = originX;
        sprite_info->OriginY = originY;
        sprite_info->Rotation = rotation;
        sprite_info->Effects = effects;
        _textureInfo[_numSprites] = texture;
        _numSprites += 1;
    }
}

void SpriteBatch::GenerateVertexInfo(
    PositionTexture4* sprite,
    const float sourceX,
    const float sourceY,
    const float sourceW,
    const float sourceH,
    const float destinationX,
    const float destinationY,
    const float destinationW,
    const float destinationH,
    const Color color,
    const float originX,
    const float originY,
    float rotation,
    SpriteEffects effects
)
{
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(destinationX - originX, destinationY - originY, 0.0f));
    model = scale(model, glm::vec3(glm::vec2(destinationW, destinationH), 1.0f));

    sprite->Position0 = model * PositionTexture4::Default().Position0;
    sprite->Position1 = model * PositionTexture4::Default().Position1;
    sprite->Position2 = model * PositionTexture4::Default().Position2;
    sprite->Position3 = model * PositionTexture4::Default().Position3;

    const auto minXRatio = sourceX;
    const auto minYRatio = sourceY;
    const auto maxXRatio = sourceX + sourceW;
    const auto maxYRatio = sourceY + sourceH;

    sprite->TextureCoordinate0 = glm::vec2(minXRatio, minYRatio);
    sprite->TextureCoordinate1 = glm::vec2(maxXRatio, minYRatio);
    sprite->TextureCoordinate2 = glm::vec2(maxXRatio, maxYRatio);
    sprite->TextureCoordinate3 = glm::vec2(minXRatio, maxYRatio);
}


void SpriteBatch::FlushBatch()
{
    PrepRenderState();
    if (_numSprites == 0)
        return;

    if (_spriteSortMode != SpriteSortMode::Deferred)
    {
        Texture2D* sorted_texture_temp[MAX_SPRITES];
        std::vector<size_t> indices(_numSprites);
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(
            indices.begin(),
            indices.end(),
            [&](const size_t i, const size_t j)
            {
                if (_spriteInfos[i].TextureID == _spriteInfos[j].TextureID)
                    return i < j; // 保持原顺序
                return _spriteInfos[i].TextureID < _spriteInfos[j].TextureID;
            }
        );
        for (int i = 0; i < _numSprites; i += 1)
        {
            _sortedSpriteInfos[i] = &_spriteInfos[indices[i]];
            sorted_texture_temp[i] = _textureInfo[indices[i]];
        }
        std::copy_n(sorted_texture_temp, _numSprites, _textureInfo);
        for (int i = 0; i < _numSprites; i += 1)
        {
            const auto* info = _sortedSpriteInfos[i];
            GenerateVertexInfo(
                &_vertexInfo[i],
                info->SourceX,
                info->SourceY,
                info->SourceW,
                info->SourceH,
                info->DestinationX,
                info->DestinationY,
                info->DestinationW,
                info->DestinationH,
                info->Color,
                info->OriginX,
                info->OriginY,
                info->Rotation,
                info->Effects
            );
        }
    }

    // #ifdef PLATFORM_WINDOWS
    int arrayOffset = 0;
nextbatch:
    const int batchSize = glm::min(_numSprites, MAX_SPRITES);
    const int baseOff = UpdateVertexBuffer(arrayOffset, batchSize);
    int offset = 0;

    Texture2D* curTexture = _textureInfo[arrayOffset];
    for (int i = 1; i < batchSize; i += 1)
    {
        if (Texture2D* tex = _textureInfo[arrayOffset + i]; tex != curTexture)
        {
            DrawPrimitivesBase(curTexture, baseOff + offset, i - offset);
            curTexture = tex;
            offset = i;
        }
    }
    DrawPrimitivesBase(curTexture, baseOff + offset, batchSize - offset);

    if (_numSprites > MAX_SPRITES)
    {
        _numSprites -= MAX_SPRITES;
        arrayOffset += MAX_SPRITES;
        goto nextbatch;
    }
    _numSprites = 0;
    /*#elifdef PLATFORM_ANDROID
        const int batchSize = _numSprites;
        int offset = 0;
        Texture2D* curTexture = _textureInfo[0];
        for (int i = 1; i < batchSize; i += 1)
        {
            if (Texture2D* tex = _textureInfo[i]; tex != curTexture)
            {
                const PositionTexture4* p = &_vertexInfo[offset];
                const int primitiveSize = i - offset;
                _bufferManager.SetData(nullptr, p, primitiveSize * sizeof(PositionTexture4));
                DrawPrimitives(curTexture, primitiveSize);
                curTexture = tex;
                offset = i;
            }
        }
        const PositionTexture4* p = &_vertexInfo[offset];
        const int primitiveSize = batchSize - offset;
        _bufferManager.SetData(nullptr, p, primitiveSize * sizeof(PositionTexture4));
        DrawPrimitives(curTexture, primitiveSize);
        _numSprites = 0;
    #endif*/
}

int SpriteBatch::UpdateVertexBuffer(const int start, const int count)
{
    int offset;
    SetDataOptions options;
    if (_bufferOffset + count > MAX_SPRITES)
    {
        offset = 0;
        options = SetDataOptions::Discard;
    }
    else
    {
        offset = _bufferOffset;
        options = SetDataOptions::NoOverwrite;
    }

    const PositionTexture4* p = &_vertexInfo[start];

    _bufferManager.SetDataPointerEXT(offset, p, count, options);

    _bufferOffset = offset + count;
    return offset;
}

void SpriteBatch::DrawPrimitives(Texture2D* texture2D, const int primitiveSize) const
{
    _graphicsDevice->Textures[0] = texture2D;
    _graphicsDevice->DrawPrimitivesIndexed(GL_TRIANGLES, primitiveSize * 6);
}

void SpriteBatch::DrawPrimitivesBase(Texture2D* texture2D, const int primitiveOffset, const int primitiveSize) const
{
    _graphicsDevice->Textures[0] = texture2D;
    _graphicsDevice->DrawPrimitivesIndexedBase(GL_TRIANGLES, primitiveOffset * 4, primitiveSize * 6);
}

void SpriteBatch::PrepRenderState()
{
    _graphicsDevice->SamplerStates[0] = _samplerState;
    _graphicsDevice->ObserveMatrix = _matrix;
    _graphicsDevice->SetBufferManager(&_bufferManager);
}

void SpriteBatch::GenerateIndexArray(short* indices)
{
    for (int i = 0, j = 0; i < MAX_INDICES; i += 6, j += 4)
    {
        indices[i] = static_cast<short>(j + 2);
        indices[i + 1] = static_cast<short>(j + 1);
        indices[i + 2] = static_cast<short>(j + 3);
        indices[i + 3] = static_cast<short>(j + 1);
        indices[i + 4] = static_cast<short>(j);
        indices[i + 5] = static_cast<short>(j + 3);
    }
}
