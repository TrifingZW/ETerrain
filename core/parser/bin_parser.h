//
// Created by TrifingZW on 24-12-4.
//

#pragma once
#include <string>
#include <vector>

#include "btl_structure.h"


class BinParser
{
public:
    BinInfo binInfo{};
    std::vector<Topography> topographies{};

    // 计算宽度和高度
    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;
    [[nodiscard]] int GetSize() const;
    [[nodiscard]] int GetCount() const;

    // 解析Bin文件
    BinParser& Parse(const std::string& name);

private:
    std::string AssetsPath = "assets"; // 资产路径
};
