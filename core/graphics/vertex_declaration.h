//
// Created by TrifingZW on 24-12-9.
//

#pragma once

struct VertexElement
{
    int Index;
    int Offset;
    int Size;
    int Type;
    bool Normalized;

    VertexElement(): Index(0), Offset(0), Size(0), Type(0), Normalized(false) {};

    VertexElement(const int usageIndex, const int offset, const int size, const int type, const bool normalized = false)
        : Index(usageIndex), Offset(offset), Size(size), Type(type), Normalized(normalized) {}
};

#include <initializer_list>

class VertexDeclaration
{
public:
    VertexElement* Elements = nullptr;
    int NumElements = 0;
    int Stride = 0;

    // 使用初始化列表参数
    VertexDeclaration(const std::initializer_list<VertexElement> elements)
        : NumElements(static_cast<int>(elements.size()))
    {
        // 分配内存
        Elements = new VertexElement[NumElements];

        // 复制元素并计算 Stride
        int i = 0;
        for (const auto& element: elements)
        {
            Elements[i] = element;
            Stride += Elements[i].Size * Elements[i].Type;
            ++i;
        }
    }

    // 析构函数释放动态分配的内存
    ~VertexDeclaration()
    {
        delete[] Elements;
    }
};
