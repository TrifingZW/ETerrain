//
// Created by TrifingZW on 24-11-30.
//

#include "sprite_batch.h"

#include <stdexcept>
#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "math/math_funcs.h"

using namespace Graphics;

SpriteBatch::SpriteBatch(GraphicsDevice* graphicsDevice): _graphicsDevice(graphicsDevice)
{
    GenerateIndexArray(_indices);
    _bufferManager.SetIndexPointerEXT(_indices, MAX_INDICES_SIZE);
}

void SpriteBatch::Begin() { Begin(SpriteSortMode::Deferred); }

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = spriteSortMode;
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, const glm::mat4& matrix)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = spriteSortMode;
    _samplerState = SamplerState();
    _matrix = matrix;
}

void SpriteBatch::Begin(SamplerState samplerState, const glm::mat4& matrix)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = SpriteSortMode::Deferred;
    _samplerState = std::move(samplerState);
    _matrix = matrix;
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, SamplerState samplerState)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = spriteSortMode;
    _samplerState = std::move(samplerState);
}

void SpriteBatch::Begin(const SpriteSortMode spriteSortMode, SamplerState samplerState, const glm::mat4& matrix)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _spriteSortMode = spriteSortMode;
    _samplerState = std::move(samplerState);
    _matrix = matrix;
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

void SpriteBatch::Draw(Texture2D* texture, const glm::vec2 position, const Rect sourceRect, const Color color)
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

void SpriteBatch::DrawCenter(Texture2D* texture, const glm::vec2 position, const Rect sourceRect, const Color color)
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
    const Rect sourceRect,
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
    const Rect sourceRect,
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

void SpriteBatch::Draw(Texture2D* texture, const Rect TargetRect, const Color color)
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

void SpriteBatch::Draw(Texture2D* texture, const Rect SourceRect, const Rect TargetRect, const Color color)
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
    const Rect SourceRect,
    const Rect TargetRect,
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

void SpriteBatch::End()
{
    if (!_beginCalled)
        throw std::runtime_error("在成功调用 Begin 之前，无法再次调用 End。");
    _beginCalled = false;

    if (_spriteSortMode != SpriteSortMode::Immediate)
        FlushBatch();
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
    if (_spriteSortMode == SpriteSortMode::Deferred)
    {
        if (_numSprites >= MAX_SPRITES)
            FlushBatch();
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
    else
    {
        if (_numSprites == 0)
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
            _textureInfo[0] = texture;
            _numSprites++;
        }
        else
        {
            if (_textureInfo[0] != texture)
                FlushBatch();
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
        }
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
            DrawPrimitives(curTexture, baseOff + offset, i - offset);
            curTexture = tex;
            offset = i;
        }
    }
    DrawPrimitives(curTexture, baseOff + offset, batchSize - offset);

    if (_numSprites > MAX_SPRITES)
    {
        _numSprites -= MAX_SPRITES;
        arrayOffset += MAX_SPRITES;
        goto nextbatch;
    }
    _numSprites = 0;
}

int SpriteBatch::UpdateVertexBuffer(const int start, const int count)
{
    int offset;
    SetDataOptions options;
    if ((_bufferOffset + count) > MAX_SPRITES)
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

void SpriteBatch::DrawPrimitives(Texture2D* texture2D, const int primitiveOffset, const int primitiveSize) const
{
    _graphicsDevice->textures[0] = texture2D;
    _graphicsDevice->DrawIndexedPrimitives(GL_TRIANGLES, primitiveOffset * 4, primitiveSize * 6);
}

void SpriteBatch::PrepRenderState()
{
    _graphicsDevice->samplerStates[0] = _samplerState;
    _graphicsDevice->observeMatrix = _matrix;
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
