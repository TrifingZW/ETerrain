//
// Created by TrifingZW on 24-11-19.
//

#pragma once
#include <cstdint>

class Topography {
public:
    uint8_t 地块类型;
    uint8_t 地块ID;
    uint8_t 地块X;
    uint8_t 地块Y;
    uint8_t 装饰类型A;
    uint8_t 装饰AID;
    uint8_t 装饰AX;
    uint8_t 装饰AY;
    uint8_t 装饰类型B;
    uint8_t 装饰BID;
    uint8_t 装饰BX;
    uint8_t 装饰BY;
    uint8_t 水边缘;
    uint8_t 路边缘;
    uint8_t 海岸;
    uint8_t empty4;
};
