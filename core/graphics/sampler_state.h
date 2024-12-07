//
// Created by TrifingZW on 24-12-7.
//

#pragma once
#include "graphics_enum.h"
#include "graphics_resource.h"


class SamplerState : public GraphicsResource
{
public:
    Graphics::TextureAddressMode AddressU, AddressV, AddressW;
    Graphics::TextureFilter Filter;

    SamplerState();

    SamplerState(
        const std::string& name,
        Graphics::TextureFilter filter,
        Graphics::TextureAddressMode addressU,
        Graphics::TextureAddressMode addressV,
        Graphics::TextureAddressMode addressW
    );

    static const SamplerState LinearClamp;
    static const SamplerState LinearWrap;
    static const SamplerState LinearMirror;
    static const SamplerState PointClamp;
    static const SamplerState PointWrap;
    static const SamplerState PointMirror;
};

