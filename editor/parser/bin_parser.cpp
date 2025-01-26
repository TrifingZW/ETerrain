//
// Created by TrifingZW on 24-12-4.
//

#include "bin_parser.h"

#include <iostream>

#include "e_terrain.h"
#include "core/helpers/assets_helper.h"

int BinParser::GetWidth() const { return binInfo.Width; }
int BinParser::GetHeight() const { return binInfo.Height; }
int BinParser::GetSize() const { return binInfo.Width * binInfo.Height; }
int BinParser::GetCount() const { return GetWidth() * GetHeight(); }

void BinParser::Parse(const std::string& name)
{
    try
    {
#ifdef PLATFORM_WINDOWS
        const std::string binPath = AssetsPath + "/" + name;
        std::ifstream stream(binPath, std::ios::binary);
        if (!stream)
            throw std::runtime_error("没有导入bin");
#elif defined(PLATFORM_ANDROID)
        std::stringstream stream = AssetsHelper::CreateBinaryStreamFromAsset(name);
        if (!stream)
            throw std::runtime_error("没有导入bin");
#endif

        char masterBytes[0x10];
        stream.read(masterBytes, sizeof(masterBytes));

        // 将 masterBytes 中的数据读入 BinInfo 结构体
        std::memcpy(&binInfo, masterBytes, sizeof(BinInfo));

        // 读取 Topographies 数组
        topographies = ETerrain::ReadToClassArray<Topography>(stream, GetCount());

        // 检查是否有剩余的数据

        // 如果需要，你可以将剩余的数据保存到某个地方
        if (std::vector remainingData((std::istreambuf_iterator(stream)), std::istreambuf_iterator<char>()); !remainingData.empty())
        {
            std::cout << "剩余数据: " << remainingData.size() << " 字节" << std::endl;
            this->remainingData = std::move(remainingData);
        }
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("解析Bin错误: " + std::string(e.what()));
    }
}

void BinParser::Save(const std::string& name)
{
    try
    {
        std::ofstream outFile(name, std::ios::binary); // 打开文件用于写入二进制数据
        if (!outFile)
            throw std::runtime_error("无法打开文件进行保存");

        // 保存 BinInfo
        outFile.write(reinterpret_cast<char *>(&binInfo), sizeof(BinInfo));

        // 保存 Topographies
        const size_t topographyCount = topographies.size();
        outFile.write(
            reinterpret_cast<char *>(topographies.data()),
            static_cast<std::streamsize>(topographyCount * sizeof(Topography))
        );

        // 保存 remainingData
        const size_t remainingDataSize = remainingData.size();
        outFile.write(remainingData.data(), static_cast<std::streamsize>(remainingDataSize));

        outFile.close(); // 关闭文件
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("保存数据时发生错误: " + std::string(e.what()));
    }
}
