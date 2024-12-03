//
// Created by TrifingZW on 24-11-30.
//

#include "sprite_batch.h"

#include <iostream>
#include <stdexcept>

#include "glm/ext/matrix_transform.hpp"

SpriteBatch::SpriteBatch(GraphicsDevice* graphicsDevice): _graphicsDevice(graphicsDevice)
{
    GenerateIndexArray(_indices);
    _bufferManager.SetIndexPointerEXT(_indices, MAX_INDICES_SIZE);
}

void SpriteBatch::Begin() { Begin(SpriteSortMode::Deferred, glm::mat4(0.1f)); }

void SpriteBatch::Begin(const SpriteSortMode sortMode, const glm::mat4& matrix)
{
    if (_beginCalled)
        throw std::runtime_error("在成功调用 End 之前，无法再次调用 Begin。");
    _beginCalled = true;

    _sortMode = sortMode;
    _matrix = matrix;
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
    PushSprite(
        texture2D,
        SourceRect.X,
        SourceRect.Y,
        SourceRect.Width,
        SourceRect.Height,
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

    if (_sortMode != SpriteSortMode::Immediate)
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
    if (_sortMode == SpriteSortMode::Deferred)
    {
        if (_numSprites >= MAX_SPRITES)
            FlushBatch();
        GenerateVertexInfo(
            &_vertexInfo[_numSprites],
            &_modelMatrix[_numSprites],
            texture->Width,
            texture->Height,
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
    } else
    {
        if (_numSprites == 0)
        {
            GenerateVertexInfo(
                &_vertexInfo[0],
                &_modelMatrix[0],
                texture->Width,
                texture->Height,
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
        } else
        {
            if (_textureInfo[0] != texture)
                FlushBatch();
            GenerateVertexInfo(
                &_vertexInfo[0],
                &_modelMatrix[0],
                texture->Width,
                texture->Height,
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
    glm::mat4* model,
    const int textureW,
    const int textureH,
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
    glm::mat4 mat(1.0f);
    mat = translate(mat, glm::vec3(destinationX + originX, destinationY + originY, 0.0f));
    mat = scale(mat, glm::vec3(glm::vec2(destinationW, destinationH), 1.0f));
    memcpy(model, &mat, sizeof(glm::mat4));

    const auto minXRatio = sourceX / static_cast<float>(textureW);
    const auto minYRatio = sourceY / static_cast<float>(textureH);
    const auto maxXRatio = (sourceX + sourceW) / static_cast<float>(textureW);
    const auto maxYRatio = (sourceY + sourceH) / static_cast<float>(textureH);
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
    } else
    {
        offset = _bufferOffset;
        options = SetDataOptions::NoOverwrite;
    }

    const PositionTexture4* p = &_vertexInfo[start];
    const glm::mat4* m = &_modelMatrix[start];

    _bufferManager.SetDataPointerEXT(offset, p, count, options);
    _bufferManager.SetMatrixPointerEXT(offset, m, count, options);

    _bufferOffset = offset + count;
    return offset;
}

void SpriteBatch::DrawPrimitives(Texture2D* texture2D, const int primitiveOffset, const int primitiveSize) const
{
    _graphicsDevice->texture2D = texture2D;
    _graphicsDevice->DrawIndexedPrimitives(GL_TRIANGLES, primitiveOffset * 4, primitiveSize * 6);
}

void SpriteBatch::PrepRenderState()
{
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
