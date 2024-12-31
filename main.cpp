#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

# ifdef BUILD_TIME
#include "font.h"
# endif

#include <glad/glad.h>

#ifdef PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include "core/core.h"

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
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

// 设置所有控件的缩放
void ApplyDpiScale(const float dpi_scale)
{
    ImGui::GetStyle().ScaleAllSizes(dpi_scale);
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
    const float dpi_scale = GetDpiScaleForWindow(hwnd);
    ApplyDpiScale(dpi_scale);

    // 加载字体
#ifdef BUILD_TIME
    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;
    ADD_FONT_FROM_MEMORY_TTF(io, 16.0f, &font_cfg);
    large_font = ADD_FONT_FROM_MEMORY_TTF(io, 24.0f, &font_cfg);
#endif

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
    Core::Instance().InitTree();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        Core::Instance().ProcessTree(currentFrame);
        Core::Instance().Input(1);

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
