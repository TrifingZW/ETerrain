//
// Created by TrifingZW on 24-12-9.
//

#pragma once

#include "vertex_declaration.h"

class IVertexType
{
public:
    // 非模板接口
    virtual size_t GetVertexCount() = 0;
    virtual size_t GetVertexMemorySize() = 0;
    virtual void* GetVertexDataPtr() = 0;
    virtual size_t GetVertexDataMemorySize() = 0;

    virtual size_t GetIndicesCount() { return 0; }
    virtual void* GetIndicesDataPtr() { return nullptr; }
    virtual size_t GetIndicesDataMemorySize() { return 0; }

    virtual VertexDeclaration& GetVertexDeclaration() = 0;
    virtual ~IVertexType() = default;
};
