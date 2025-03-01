#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <windows.h>

#include <glad/glad.h>

#ifdef PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <e_terrain.h>
#include <thorvg.h>

#include "core/core.h"
#include "core/icon/IconsFontAwesome6.h"
#include "core/icon/IconsMaterialDesign.h"

ImFont* large_font = nullptr; // 定义全局字体指针
constexpr auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// 创建窗口
GLFWwindow* GetGlfwWindow()
{
    static GLFWwindow* window = glfwCreateWindow(1280, 720, "ETerrain", nullptr, nullptr);
    return window;
}

// 处理错误信息
void GlfwErrorCallback(const int error, const char* description)
{
    const int result = fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    std::cout << result << '\n';
}

// 获取系统 DPI 缩放
float GetDpiScaleForWindow(HWND hwnd)
{
    const UINT dpi = GetDpiForWindow(hwnd);
    constexpr UINT default_dpi = 96; // 标准 DPI 值为 96
    return static_cast<float>(dpi) / default_dpi;
}

void InitThorvg()
{
    tvg::Initializer::init(tvg::CanvasEngine::Sw, 0); //thread count
}

// 初始化 Dear ImGui
void InitImGui(GLFWwindow* window, const char* glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控件
    //    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // 启用游戏手柄控件
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 启用停靠功能
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 启用多视口支持

    // 设置dpi为系统dpi
    HWND hwnd = glfwGetWin32Window(window);
    ETerrain::DpiScale = GetDpiScaleForWindow(hwnd);
    ETerrain::FontScale = 16 * ETerrain::DpiScale;
    ImGui::GetStyle().ScaleAllSizes(ETerrain::DpiScale);
    io.Fonts->AddFontFromFileTTF(
        "fonts/HarmonyOS_Sans_SC_Bold.ttf",
        ETerrain::FontScale,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull()
    );
    static ImWchar material_ranges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
    static ImWchar awesome6_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig cfg;
    cfg.MergeMode = true;
    cfg.PixelSnapH = true;
    cfg.OversampleH = 1;
    cfg.OversampleV = 1;
    cfg.GlyphMinAdvanceX = ETerrain::FontScale;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_MD, ETerrain::FontScale, &cfg, material_ranges);
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAR, ETerrain::FontScale, &cfg, awesome6_ranges);
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, ETerrain::FontScale, &cfg, awesome6_ranges);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

// 渲染 ImGui 内容
void RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Core::Instance().ImGuiAll();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// 渲染 OpenGL 内容
void RenderOpenGl()
{
    glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Core::Instance().Renderings();
}

// 退出程序时清理
void Shutdown(GLFWwindow* window)
{
    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int, char**)
{
    // 初始化 GLFW
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) return 1;

    const auto glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ETerrain", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 启用 vsync
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, const int width, const int height) { glViewport(0, 0, width, height); });

    // 初始化 GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    // 初始化 ImGui 和 Core
    InitImGui(window, glsl_version);
    InitThorvg();
    Core::Instance().InitTree();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        static double deltaTime; // 当前帧与上一帧的时间差
        static double lastFrame; // 上一帧的时间
        const auto currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Core::Instance().ProcessTree(deltaTime);

        // 如果窗口最小化，暂停一段时间以减少无效计算
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
        {
            glfwWaitEventsTimeout(0.1);
            continue;
        }

        // 渲染 OpenGL 和 ImGui 内容
        RenderOpenGl();
        RenderImGui();

        // 交换缓冲区
        glfwSwapBuffers(window);

        if (const GLenum status = glGetError(); status != GL_NO_ERROR)
            std::cerr << "GL_ERROR:" << status << std::endl;
    }

    Shutdown(window);
    return 0;
}
