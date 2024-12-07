//
// Created by TrifingZW on 24-12-8.
//

#include "texture_collection.h"

TextureCollection::TextureCollection(const int slots): slots(slots)
{
    textures = new Texture2D*[slots]();
}
