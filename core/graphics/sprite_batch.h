//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <string>

#include "buffer_manager.h"
#include "graphics_device.h"
#include "scene/resources/texture_2d.h"
#include "graphics_enum.h"
#include "graphics_structure.h"
#include "glm/fwd.hpp"

class SpriteBatch
{
public:
    static constexpr int MAX_SPRITES = 2048;

private:
    GraphicsDevice* _graphicsDevice = nullptr;
    BufferManager _bufferManager = BufferManager(MAX_SPRITES * 5 * 4, MAX_SPRITES * 6, MAX_SPRITES * 4 * sizeof(glm::mat4));

    PositionTexture4 _vertexInfo[MAX_SPRITES] = {};
    glm::mat4 _modelMatrix[MAX_SPRITES] = {};
    Texture2D* _textureInfo[MAX_SPRITES] = {};

    SpriteSortMode _sortMode = SpriteSortMode::Deferred;
    bool _beginCalled = false;
    int _numSprites = 0;
    int _bufferOffset = 0;

public:
    explicit SpriteBatch(GraphicsDevice* graphicsDevice);

    void Begin();
    void Begin(SpriteSortMode sortMode);
    void Draw(
        const Texture2D& texture2D,
        Rect SourceRect,
        Rect TargetRect,
        Color color,
        float rotation,
        glm::vec2 origin,
        SpriteEffects effects
    );
    void End();

private:
    void CheckBegin(const std::string& method) const;
    void PushSprite(
        Texture2D texture,
        float sourceX,
        float sourceY,
        float sourceW,
        float sourceH,
        float destinationX,
        float destinationY,
        float destinationW,
        float destinationH,
        Color color,
        float originX,
        float originY,
        float rotation,
        SpriteEffects effects
    );
    void FlushBatch();
    int UpdateVertexBuffer(int start, int count);
    void DrawPrimitives(Texture2D* texture2D, int i, int i1) const;
    void PrepRenderState();

    static void GenerateVertexInfo(
        PositionTexture4* sprite,
        glm::mat4* model,
        int textureW,
        int textureH,
        float sourceX,
        float sourceY,
        float sourceW,
        float sourceH,
        float destinationX,
        float destinationY,
        float destinationW,
        float destinationH,
        Color color,
        float originX,
        float originY,
        float rotation,
        SpriteEffects effects
    );
};
