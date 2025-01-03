//
// Created by TrifingZW on 24-11-19.
//

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/graphics/graphics_structure.h"
#include "scene/resources/texture_2d.h"

struct Image
{
    std::string Name;
    int X, Y, W, H;
    int Refx, Refy; // Reference points
};

class ResourceTextureParser
{
public:
    std::string FilePath;
    std::string TextureName{};
    std::vector<Image> Images{};
    Texture2D Texture2D{};

    explicit ResourceTextureParser(std::string filePath);

    std::optional<Rect> GetRect(const std::string& imageName);
    static Rect GetRect(const Image& image);
    [[nodiscard]] std::vector<float> GetVertices(const std::string& imageName);

private:
    bool loadFromXml();
    void loadTexture();
};
