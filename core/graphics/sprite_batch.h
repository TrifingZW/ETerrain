//
// Created by TrifingZW on 24-11-23.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics_device.h"
#include "sprite_sortmode.h"
#include "scene/resources/texture_2d.h"

class SpriteBatch {
    GraphicsDevice _graphicsDevice;

    SpriteSortMode _sortMode = SpriteSortMode::Deferred;

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec4 color;
    };

    struct Sprite {
        Texture2D texture;
        Vertex vertices[4]{};
        float depth{};
    };

    struct Rectangle {
    };

    explicit SpriteBatch(const GraphicsDevice &graphicsDevice);

    void Begin(SpriteSortMode sortMode);
    void Draw(Texture2D &texture, glm::vec2 position, glm::vec4 color);
    void Draw(Texture2D &texture, glm::vec2 position, Rectangle sourceRectangle, glm::vec4 color);
    void Draw(Texture2D &texture, glm::vec2 position, Rectangle sourceRectangle, glm::vec4 color, float rotation, glm::vec2 origin, float scale);
    void Draw(Texture2D &texture, glm::vec2 position, Rectangle sourceRectangle, glm::vec4 color, float rotation, glm::vec2 origin, glm::vec2 scale);
    void Draw(Texture2D &texture, Rectangle destinationRectangle, glm::vec2 color);
    void Draw(Texture2D &texture, Rectangle destinationRectangle, Rectangle sourceRectangle, glm::vec2 color);
    void Draw(Texture2D &texture, Rectangle destinationRectangle, Rectangle sourceRectangle, glm::vec2 color, float rotation, glm::vec2 origin);
    void End();

private:
    void FlushBatch();
};
