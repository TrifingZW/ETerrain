//
// Created by TrifingZW on 24-11-19.
//

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/math/rect2.h"
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

    [[nodiscard]] std::optional<Rect2> GetRect2(const std::string& imageName) const;
    [[nodiscard]] std::optional<Image> GetImage(const std::string& imageName) const;
    [[nodiscard]] std::vector<float> GetVertices(const std::string& imageName);

    [[nodiscard]] static Rect2 GetRect2(const Image& image);

private:
    bool loadFromXml();
    void loadTexture();
};
