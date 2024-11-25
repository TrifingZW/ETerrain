//
// Created by TrifingZW on 24-11-17.
//

#ifndef FONT_H
#define FONT_H

#ifdef BUILD_TIME
#include "harmony_os_sans_sc_bold.h"
#define ADD_FONT_FROM_MEMORY_TTF(io, scale, font_cfg) \
io.Fonts->AddFontFromMemoryTTF(font_ttf, static_cast<int>(font_ttf_len), scale * dpi_scale, font_cfg, io.Fonts->GetGlyphRangesChineseFull())
#else
#define ADD_FONT_FROM_MEMORY_TTF(io, scale) // 空定义
#endif

#endif // FONT_H