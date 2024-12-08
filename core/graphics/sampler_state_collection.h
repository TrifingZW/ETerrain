//
// Created by TrifingZW on 24-12-8.
//

#pragma once
#include <stdexcept>

#include "sampler_state.h"


class SamplerStateCollection
{
public:
    int slots = 0;
    explicit SamplerStateCollection(int slots);

    // 重载operator[]，使得可以通过索引访问纹理
    SamplerState& operator[](const int index)
    {
        if (index < 0 || index >= slots)
            throw std::out_of_range("Index out of range");

        return samplerStates[index];
    }

    SamplerState operator[](const int index) const
    {
        if (index < 0 || index >= slots)
            throw std::out_of_range("Index out of range");

        return samplerStates[index];
    }

private:
    SamplerState* samplerStates = nullptr;
};
