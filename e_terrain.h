//
// Created by TrifingZW on 24-12-4.
//

#pragma once

#include <fstream>
#include <vector>

namespace ETerrain
{
    // 读取数据到类数组（模板函数）
    template<typename T>
    std::vector<T> ReadToClassArray(std::ifstream& stream, int count)
    {
        std::vector<T> result(count);
        stream.read(reinterpret_cast<char *>(result.data()), count * sizeof(T));
        if (!stream)
            throw std::runtime_error("读取类数组失败");
        return result;
    }
}
