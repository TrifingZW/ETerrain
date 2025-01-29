//
// Created by TrifingZW on 24-12-4.
//

#pragma once

#include <cstdint>

struct BinInfo
{
    uint8_t Unknown1;
    uint8_t Unknown2;
    uint8_t Unknown3;
    uint8_t Unknown4;
    uint8_t Unknown5;
    uint8_t Unknown6;
    uint8_t Unknown7;
    uint8_t Unknown8;
    int32_t Width;
    int32_t Height;
};

struct Topography
{
    uint8_t 地块类型;
    uint8_t 地块ID;
    int8_t 地块X;
    int8_t 地块Y;
    uint8_t 装饰类型A;
    uint8_t 装饰AID;
    int8_t 装饰AX;
    int8_t 装饰AY;
    uint8_t 装饰类型B;
    uint8_t 装饰BID;
    int8_t 装饰BX;
    int8_t 装饰BY;
    uint8_t 水边缘;
    uint8_t 路边缘;
    uint8_t 海岸;
    uint8_t empty4;
};
