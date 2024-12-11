//
// Created by TrifingZW on 24-12-9.
//

#pragma once

#include "vertex_declaration.h"

class IVertexType
{
public:
    // 非模板接口
    virtual int GetVertexCount() = 0;
    virtual int GetVertexMemorySize() = 0;
    virtual int GetDataMemorySize() = 0;
    virtual void* GetVertexDataPtr() = 0;
    virtual VertexDeclaration GetVertexDeclaration() = 0;
    virtual ~IVertexType() = default;
};
