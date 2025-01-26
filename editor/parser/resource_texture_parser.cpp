//
// Created by TrifingZW on 24-11-19.
//

#include "resource_texture_parser.h"

#include <pugixml.hpp>
#include <filesystem>

#include "core/helpers/assets_helper.h"

std::string getParentPath(const std::string& filePath)
{
    const std::filesystem::path path(filePath);
    return path.parent_path().string();
}

ResourceTextureParser::ResourceTextureParser(std::string filePath): FilePath(std::move(filePath))
{
    loadFromXml();
    loadTexture();
}

std::optional<Rect2> ResourceTextureParser::GetRect(const std::string& imageName) const
{
    const auto image = std::find_if(
        Images.begin(),
        Images.end(),
        [imageName](const Image& item) { return item.Name == imageName; }
    );

    if (image == Images.end())
        return std::nullopt; // 如果没有找到，返回空值

    return GetRect2(*image); // 找到时返回实际的 Rect2
}

Rect2 ResourceTextureParser::GetRect2(const Image& image) const
{
    return {static_cast<float>(image.X), static_cast<float>(image.Y), static_cast<float>(image.W), static_cast<float>(image.H)};
}

std::vector<float> ResourceTextureParser::GetVertices(const std::string& imageName)
{
    const auto image = std::find_if(
        Images.begin(),
        Images.end(),
        [imageName](const Image& item) { return item.Name == imageName; }
    );

    if (image != Images.end())
    {
        auto minXRatio = static_cast<float>(image->X) / static_cast<float>(Texture2D.Width);
        auto minYRatio = static_cast<float>(image->Y) / static_cast<float>(Texture2D.Height);
        auto maxXRatio = static_cast<float>(image->X + image->W) / static_cast<float>(Texture2D.Width);
        auto maxYRatio = static_cast<float>(image->Y + image->H) / static_cast<float>(Texture2D.Height);

        // @formatter:off
        return {
            // positions      // texCoords
            0.0f,  0.0f,      minXRatio, minYRatio,  // 左下
            1.0f,  0.0f,      maxXRatio, minYRatio,  // 右下
            1.0f,  1.0f,      maxXRatio, maxYRatio,  // 右上
            0.0f,  1.0f,      minXRatio, maxYRatio   // 左上
        };
        // @formatter:on
    }
    return {0, 0, 0, 0, 0, 0, 0, 0};
}

bool ResourceTextureParser::loadFromXml()
{
    pugi::xml_document doc;
    const std::string filePath = FilePath + ".xml";
#ifdef PLATFORM_WINDOWS
    if (const pugi::xml_parse_result result = doc.load_file(("assets/" + filePath).c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#elif defined(PLATFORM_ANDROID)
    std::string xmlData;
    AssetsHelper::LoadStringFromAndroidAssets(xmlData, filePath);
    if (const pugi::xml_parse_result result = doc.load_string(xmlData.c_str()); !result)
        throw std::runtime_error("Failed to load XML file: " + filePath);
#endif

    const auto resourceNode = doc.child("Resource");
    if (!resourceNode)
    {
        throw std::runtime_error("No <Resource> root element found");
    }

    // Get the texture name
    const auto textureNode = resourceNode.child("Texture");
    if (!textureNode)
    {
        throw std::runtime_error("No <Texture> element found");
    }
    TextureName = textureNode.attribute("name").as_string();

    // Parse <Image> elements
    const auto imagesNode = resourceNode.child("Images");
    if (!imagesNode)
    {
        throw std::runtime_error("No <Images> element found");
    }

    for (auto imageNode: imagesNode.children("Image"))
    {
        Image image;
        image.Name = imageNode.attribute("name").as_string();
        image.X = imageNode.attribute("x").as_int();
        image.Y = imageNode.attribute("y").as_int();
        image.W = imageNode.attribute("w").as_int();
        image.H = imageNode.attribute("h").as_int();
        image.Refx = imageNode.attribute("refx").as_int();
        image.Refy = imageNode.attribute("refy").as_int();

        Images.push_back(image);
    }

    return true;
}

void ResourceTextureParser::loadTexture()
{
    // 构造纹理文件的完整路径
    const std::string texturePath = TextureName;

#ifdef PLATFORM_WINDOWS
    AssetsHelper::LoadTexture2DFromPath(Texture2D, "assets/" + texturePath);
#elif defined(PLATFORM_ANDROID)
    AssetsHelper::LoadTexture2DFromAndroidAssets(Texture2D, texturePath);
#endif
}
