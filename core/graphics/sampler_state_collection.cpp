//
// Created by TrifingZW on 24-12-8.
//

#include "sampler_state_collection.h"

SamplerStateCollection::SamplerStateCollection(const int slots): slots(slots)
{
    samplerStates = new SamplerState[slots]{SamplerState::LinearWrap};
}
