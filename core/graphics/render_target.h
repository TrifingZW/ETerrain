//
// Created by TrifingZW on 24-11-25.
//

#pragma once
#include "graphics_device.h"


class RenderTarget {
    GraphicsDevice _graphicsDevice;

    RenderTarget(const GraphicsDevice &graphicsDevice, int width, int height);

};
