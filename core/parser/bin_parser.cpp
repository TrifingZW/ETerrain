//
// Created by TrifingZW on 24-12-4.
//

#include "bin_parser.h"

#include "../../e_terrain.h"

int BinParser::GetWidth() const { return binInfo.Width; }
int BinParser::GetHeight() const { return binInfo.Height; }
int BinParser::GetSize() const { return binInfo.Width + binInfo.Height; }
int BinParser::GetCount() const { return GetWidth() * GetHeight(); }

BinParser& BinParser::Parse(const std::string& name)
{
    try
    {
        const std::string binPath = AssetsPath + "/" + name;
        std::ifstream stream(binPath, std::ios::binary);
        if (!stream)
            throw std::runtime_error("没有导入bin");

        char masterBytes[0x10];
        stream.read(masterBytes, sizeof(masterBytes));
        if (!stream)
            throw std::runtime_error("无法读取文件");

        // 将 masterBytes 中的数据读入 BinInfo 结构体
        std::memcpy(&binInfo, masterBytes, sizeof(BinInfo));

        // 读取 Topographies 数组
        topographies = ETerrain::ReadToClassArray<Topography>(stream, GetCount());
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("解析Bin错误: " + std::string(e.what()));
    }

    return *this;
}
