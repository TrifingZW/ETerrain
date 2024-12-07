//
// Created by TrifingZW on 24-12-7.
//

#include "sampler_state.h"

using namespace Graphics;

const SamplerState SamplerState::LinearClamp(
    "SamplerState.LinearClamp",
    TextureFilter::Linear,
    TextureAddressMode::Clamp,
    TextureAddressMode::Clamp,
    TextureAddressMode::Clamp
);

const SamplerState SamplerState::LinearWrap(
    "SamplerState.LinearClamp",
    TextureFilter::Linear,
    TextureAddressMode::Wrap,
    TextureAddressMode::Wrap,
    TextureAddressMode::Wrap
);

const SamplerState SamplerState::LinearMirror(
    "SamplerState.LinearClamp",
    TextureFilter::Linear,
    TextureAddressMode::Mirror,
    TextureAddressMode::Mirror,
    TextureAddressMode::Mirror
);

const SamplerState SamplerState::PointClamp(
    "SamplerState.LinearClamp",
    TextureFilter::Point,
    TextureAddressMode::Clamp,
    TextureAddressMode::Clamp,
    TextureAddressMode::Clamp
);

const SamplerState SamplerState::PointWrap(
    "SamplerState.LinearClamp",
    TextureFilter::Point,
    TextureAddressMode::Wrap,
    TextureAddressMode::Wrap,
    TextureAddressMode::Wrap
);

const SamplerState SamplerState::PointMirror(
    "SamplerState.LinearClamp",
    TextureFilter::Point,
    TextureAddressMode::Mirror,
    TextureAddressMode::Mirror,
    TextureAddressMode::Mirror
);

SamplerState::SamplerState()
{
    Filter = TextureFilter::Linear;
    AddressU = TextureAddressMode::Wrap;
    AddressV = TextureAddressMode::Wrap;
    AddressW = TextureAddressMode::Wrap;
}

SamplerState::SamplerState(
    const std::string& name,
    const TextureFilter filter,
    const TextureAddressMode addressU,
    const TextureAddressMode addressV,
    const TextureAddressMode addressW
)
{
    this->name = name;
    Filter = filter;
    AddressU = addressU;
    AddressV = addressV;
    AddressW = addressW;
}
