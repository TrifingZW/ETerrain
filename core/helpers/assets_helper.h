//
// Created by TrifingZW on 24-12-7.
//

#pragma once

#include <sstream>
#include <string>

#include <stb/stb_image.h>

#ifdef PLATFORM_ANDROID
#include <imgui.h>
#include <android/asset_manager.h>
#include "platform/android/android.h"
#endif

#include <thorvg.h>

#include "scene/resources/texture_2d.h"

namespace AssetsHelper
{
    inline void LoadTexture2DFromPath(Texture2D& texture2D, const std::string& texturePath)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
        if (!data)
            throw std::runtime_error("Failed to load texture: " + texturePath);

        texture2D.Generate(width, height, channels, data);

        // 释放图像内存
        stbi_image_free(data);

        // std::cout << "Loaded texture: " << TextureName << " (ID: " << texture2D.Id << ")\n";
    }

    inline void LoadTexture2DFromPtr(Texture2D& texture2D, void* texturePtr, const int len)
    {
        int width, height, channels;
        unsigned char* data = stbi_load_from_memory(
            static_cast<stbi_uc *>(texturePtr),
            len,
            &width,
            &height,
            &channels,
            0
        );
        if (!data)
            throw std::runtime_error("Failed to load texture: ");

        texture2D.Generate(width, height, channels, data);

        // 释放图像内存
        stbi_image_free(data);

        // std::cout << "Loaded texture: " << TextureName << " (ID: " << texture2D.Id << ")\n";
    }

    inline void LoadSVGToTexture2DFromFile(Texture2D& texture2D, const std::string& svgPath, const float p_scale)
    {
        auto picture = tvg::Picture::gen();
        picture->load(svgPath);
        tvg::Result result = picture->load(svgPath);
        if (result != tvg::Result::Success) {}

        float fw, fh;
        picture->size(&fw, &fh);
        const int width = std::max(1, static_cast<int>(round(fw * p_scale)));
        const int height = std::max(1, static_cast<int>(round(fh * p_scale)));
        picture->size(static_cast<float>(width), static_cast<float>(height));

        const auto sw_canvas = tvg::SwCanvas::gen();
        auto* buffer = new uint32_t[width * height];
        sw_canvas->target(buffer, width, width, height, tvg::SwCanvas::ABGR8888S);
        sw_canvas->push(std::move(picture));
        sw_canvas->draw();
        sw_canvas->sync();
        texture2D.Generate(width, height, 4, buffer);
        sw_canvas->clear(true);
    }

#ifdef PLATFORM_ANDROID
    // 检索放入 asset/ 目录 (android/app/src/main/assets) 中的数据的函数
    inline size_t GetAssetData(const std::string& filename, void** outData)
    {
        size_t num_bytes_read = 0;
        if (AAsset* asset_descriptor = AAssetManager_open(Android::g_App->activity->assetManager, filename.c_str(), AASSET_MODE_BUFFER))
        {
            size_t num_bytes = 0;
            num_bytes = AAsset_getLength(asset_descriptor);
            *outData = IM_ALLOC(num_bytes);
            num_bytes_read = AAsset_read(asset_descriptor, *outData, num_bytes);
            AAsset_close(asset_descriptor);
            IM_ASSERT(num_bytes_read == num_bytes);
        }
        return num_bytes_read;
    }

    inline void LoadTexture2DFromAndroidAssets(Texture2D& texture2D, const std::string& texturePath)
    {
        void* imageData = nullptr;
        const size_t dataSize = GetAssetData(texturePath, &imageData);
        LoadTexture2DFromPtr(texture2D, imageData, static_cast<int>(dataSize));
    }

    inline void LoadStringFromAndroidAssets(std::string& str, const std::string& path)
    {
        void* data = nullptr;
        const size_t dataSize = GetAssetData(path, &data);
        str = std::string(static_cast<char *>(data), dataSize);
        IM_FREE(data);
    }

    inline std::stringstream CreateBinaryStreamFromAsset(const std::string& filename)
    {
        void* asset_data = nullptr;
        const size_t asset_size = GetAssetData(filename, &asset_data);

        // 使用 std::vector<char> 存储内存数据
        const std::vector buffer(static_cast<const char *>(asset_data), static_cast<const char *>(asset_data) + asset_size);

        // 将 buffer 转换为 stringstream 来处理字节流
        std::stringstream ss;
        ss.write(buffer.data(), buffer.size());

        // 可选：释放内存
        IM_FREE(asset_data);

        return ss; // 返回字节流
    }
#endif
}
