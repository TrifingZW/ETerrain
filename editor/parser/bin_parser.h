//
// Created by TrifingZW on 24-12-4.
//

#pragma once

#include <string>
#include <vector>

#include "../world/btl_structure.h"

class BinParser
{
public:
    BinInfo binInfo{};
    std::vector<Topography> topographies{};
    std::vector<char> remainingData{};

    // 计算宽度和高度
    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;
    [[nodiscard]] int GetSize() const;
    [[nodiscard]] int GetCount() const;

    // 解析Bin文件
    void Parse(const std::string& name);
    void Save(const std::string& name);

private:
    std::string AssetsPath = "package/assets"; // 资产路径
};
