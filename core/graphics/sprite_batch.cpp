//
// Created by TrifingZW on 24-11-23.
//

#include "sprite_batch.h"

SpriteBatch::SpriteBatch(const GraphicsDevice &graphicsDevice): _graphicsDevice(graphicsDevice) {
}

void SpriteBatch::Begin(const SpriteSortMode sortMode) {
    _sortMode = sortMode;
}
