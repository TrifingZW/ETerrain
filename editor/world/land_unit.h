//
// Created by TrifingZW on 24-12-28.
//

#pragma once

#include "core/math/vector2.h"
#include "core/math/vector2i.h"
#include "btl_structure.h"


class LandUnit
{
public:
    Topography* Topography = nullptr;
    Vector2 Position;
    Vector2I GridPosition;
};
