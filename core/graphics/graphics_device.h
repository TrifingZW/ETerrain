//
// Created by TrifingZW on 24-11-24.
//

#pragma once
#include "render_target.h"


class GraphicsDevice {
public:
    void Clear();
    void SetRenderTarget(RenderTarget &renderTarget);
};
