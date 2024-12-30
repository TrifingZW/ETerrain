//
// Created by TrifingZW on 24-12-31.
//

#pragma once
#include <android_native_app_glue.h>
#include <string>
#include <EGL/egl.h>

namespace Android
{
    // 数据
    inline auto g_EglDisplay = EGL_NO_DISPLAY;
    inline auto g_EglSurface = EGL_NO_SURFACE;
    inline auto g_EglContext = EGL_NO_CONTEXT;
    inline android_app* g_App = nullptr;
    inline bool g_Initialized = false;
    inline char g_LogTag[] = "ImGuiExample";
    inline std::string g_IniFilename;
}
