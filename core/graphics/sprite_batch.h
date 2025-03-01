//
// Created by TrifingZW on 24-11-30.
//

#pragma once

#include <string>

#include "scene/resources/texture_2d.h"
#include "buffer_manager.h"
#include "graphics_device.h"
#include "graphics_enum.h"
#include "graphics_structure.h"
#include "graphics_resource.h"
#include "sampler_state.h"
#include "core/math/color.h"
#include "core/math/rect2.h"

struct SpriteInfo
{
    int TextureID{};
    float SourceX{};
    float SourceY{};
    float SourceW{};
    float SourceH{};
    float DestinationX{};
    float DestinationY{};
    float DestinationW{};
    float DestinationH{};
    Color Color{};
    float OriginX{};
    float OriginY{};
    float Rotation{};
    Graphics::SpriteEffects Effects = Graphics::SpriteEffects::None;
};

class SpriteBatch : public GraphicsResource
{
public:
    static constexpr int MAX_SPRITES = 4096;
    static constexpr int MAX_VERTICES = MAX_SPRITES * 4;
    static constexpr int MAX_INDICES = MAX_SPRITES * 6;
    static constexpr GLsizeiptr MAX_VERTICES_SIZE = MAX_SPRITES * sizeof(PositionTexture4);
    static constexpr GLsizeiptr MAX_INDICES_SIZE = MAX_INDICES * sizeof(short);

private:
    GraphicsDevice* _graphicsDevice = nullptr;
    BufferManager _bufferManager = BufferManager(MAX_VERTICES_SIZE, MAX_INDICES_SIZE);

    SpriteInfo _spriteInfos[MAX_SPRITES] = {};
    SpriteInfo* _sortedSpriteInfos[MAX_SPRITES] = {};
    PositionTexture4 _vertexInfo[MAX_SPRITES] = {};
    Texture2D* _textureInfo[MAX_SPRITES] = {};
    short* _indices = new short[MAX_INDICES];

    Graphics::SpriteSortMode _spriteSortMode = Graphics::SpriteSortMode::Deferred;
    SamplerState _samplerState{};
    glm::mat4 _matrix = glm::mat4(1.0f);
    bool _beginCalled = false;
    int _numSprites = 0;
    int _bufferOffset = 0;

public:
    explicit SpriteBatch(GraphicsDevice* graphicsDevice);
    ~SpriteBatch();

    void Begin();
    void Begin(Graphics::SpriteSortMode spriteSortMode);
    void Begin(Graphics::SpriteSortMode spriteSortMode, const glm::mat4& matrix);
    void Begin(const SamplerState& samplerState, const glm::mat4& matrix);
    void Begin(Graphics::SpriteSortMode spriteSortMode, const SamplerState& samplerState);
    void Begin(Graphics::SpriteSortMode spriteSortMode, SamplerState samplerState, const glm::mat4& matrix);

    void Draw(Texture2D* texture, glm::vec2 position, Color color);
    void DrawCenter(Texture2D* texture, glm::vec2 position, Color color);
    void Draw(Texture2D* texture, glm::vec2 position, Rect2 sourceRect, Color color);
    void DrawCenter(Texture2D* texture, glm::vec2 position, Rect2 sourceRect, Color color);
    void Draw(
        Texture2D* texture,
        glm::vec2 position,
        Rect2 sourceRect,
        Color color,
        float rotation,
        glm::vec2 origin,
        float scale,
        Graphics::SpriteEffects effects
    );
    void Draw(
        Texture2D* texture,
        glm::vec2 position,
        Rect2 sourceRect,
        Color color,
        float rotation,
        glm::vec2 origin,
        glm::vec2 scale,
        Graphics::SpriteEffects effects
    );
    void Draw(Texture2D* texture, Rect2 TargetRect, Color color);
    void Draw(Texture2D* texture, Rect2 SourceRect, Rect2 TargetRect, Color color);
    void Draw(
        Texture2D* texture2D,
        Rect2 SourceRect,
        Rect2 TargetRect,
        Color color,
        float rotation,
        glm::vec2 origin,
        Graphics::SpriteEffects effects
    );
    void DrawRect(Texture2D texture, Rect2 rect, Color color);

    void End();
    void Clear();

private:
    void CheckBegin(const std::string& method) const;
    void PushSprite(
        Texture2D* texture,
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
        Graphics::SpriteEffects effects
    );
    void FlushBatch();
    int UpdateVertexBuffer(int start, int count);
    void DrawPrimitives(Texture2D* texture2D, int primitiveSize) const;
    void DrawPrimitivesBase(Texture2D* texture2D, int primitiveOffset, int primitiveSize) const;
    void PrepRenderState();
    static void GenerateIndexArray(short* indices);

    static void GenerateVertexInfo(
        PositionTexture4* sprite,
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
        Graphics::SpriteEffects effects
    );
};

