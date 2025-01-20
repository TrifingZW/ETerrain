#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

#ifdef PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <imgui_internal.h>

#include "core/core.h"

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
    ImGui::GetStyle().ScaleAllSizes(dpi_scale);
    io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/msyh.ttc",
        16 * dpi_scale,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull()
    );

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

// 渲染 ImGui 内容
void RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 窗口的 ID 和 标题
ImGuiID dockspaceID         = ImGui::GetID("##ui.dock_space");
const char* UI_DOCK_WINDOW  = "##ui.dock_window";
const char* UI_PROJECT_BOX  = u8"工程##ui.project";
const char* UI_PROPERTY_BOX = u8"属性##ui.property";
const char* UI_TOOL_BOX     = u8"工具##ui.tools";
const char* UI_MESSAGE_BOX  = u8"消息##ui.message";
const char* UI_LOG_BOX      = u8"日志##ui.log";
const char* UI_VIEW_BOX     = u8"##ui.view";

const ImGuiViewport* viewport = ImGui::GetMainViewport();
ImGui::SetNextWindowPos(viewport->WorkPos);
ImGui::SetNextWindowSize(viewport->WorkSize);
ImGui::SetNextWindowViewport(viewport->ID);

int windowFlags = ImGuiWindowFlags_NoDecoration            // 无标题栏、不可改变大小、无滚动条、不可折叠
                  | ImGuiWindowFlags_NoMove                // 不可移动
                  | ImGuiWindowFlags_NoBackground          // 无背景（背景透明）
                  | ImGuiWindowFlags_MenuBar               // 菜单栏
                  | ImGuiWindowFlags_NoDocking             // 不可停靠
                  | ImGuiWindowFlags_NoBringToFrontOnFocus // 无法设置前台和聚焦
                  | ImGuiWindowFlags_NoNavFocus            // 无法通过键盘和手柄聚焦
    ;

// 压入样式设置
ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);            // 无边框
ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // 无边界
ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);              // 无圆角
// ImGui::SetNextWindowBgAlpha(0.0f); // 窗口 alpha 为 0，同样可以不显示背景

ImGui::Begin(UI_DOCK_WINDOW, 0, windowFlags); // 开始停靠窗口
ImGui::PopStyleVar(3);                        // 弹出样式设置

// 创建停靠空间
if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) { // 判断是否开启停靠
    // 判断是否有根节点，防止一直重建
    if (!ImGui::DockBuilderGetNode(dockspaceID)) {
        // 移除根节点
        ImGui::DockBuilderRemoveNode(dockspaceID);

        // 创建根节点
        ImGuiID root = ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

        // 设置根节点位置大小
        ImGui::DockBuilderSetNodePos(root, { 0.0f, 0.0f });
        ImGui::DockBuilderSetNodeSize(root, ImGui::GetWindowSize());

        // 分割停靠空间

        // 根节点分割左节点
        ImGuiID leftNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Left, 0.25f, nullptr, &root);

        // 根节点分割右节点
        ImGuiID rightNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.25f / 0.75f, nullptr, &root);

        // 根节点分割下节点
        ImGuiID bottomNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Down, 0.25f, nullptr, &root);

        // 左节点分割上下节点
        ImGuiID leftTopNode, leftBottomNode;
        ImGui::DockBuilderSplitNode(leftNode, ImGuiDir_Up, 0.5f, &leftTopNode, &leftBottomNode);

        // 设置节点属性

        // 禁止其他窗口/节点分割根节点
        ImGui::DockBuilderGetNode(dockspaceID)->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplit;

        // 设置分割到最后的根节点隐藏标签栏
        ImGui::DockBuilderGetNode(root)->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;

        // 设置节点停靠窗口
        ImGui::DockBuilderDockWindow(UI_PROJECT_BOX, leftTopNode);     // 左上节点
        ImGui::DockBuilderDockWindow(UI_PROPERTY_BOX, leftBottomNode); // 左下节点
        ImGui::DockBuilderDockWindow(UI_TOOL_BOX, rightNode);          // 右边节点

        ImGui::DockBuilderDockWindow(UI_MESSAGE_BOX, bottomNode);      // 下面节点同时停靠两个窗口
        ImGui::DockBuilderDockWindow(UI_LOG_BOX, bottomNode);

        ImGui::DockBuilderDockWindow(UI_VIEW_BOX, root); // 观察窗口平铺“客户区”，停靠的节点是 CentralNode

        // 结束停靠设置
        ImGui::DockBuilderFinish(dockspaceID);

        // 设置焦点窗口
        ImGui::SetWindowFocus(UI_VIEW_BOX);
    }

    // 创建停靠空间
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
ImGui::End(); // 结束停靠窗口

// 工程框
if (ImGui::Begin(UI_PROJECT_BOX)) {
    ImGui::LabelText("label", "text");
    ImGui::Button("button");
}
ImGui::End();

// 属性框
if (ImGui::Begin(UI_PROPERTY_BOX)) {
    ImGui::LabelText("label", "text");
    ImGui::Button("button");
}
ImGui::End();

// 工具框
if (ImGui::Begin(UI_TOOL_BOX)) {
    ImGui::LabelText("label", "text");
    ImGui::Button("button");

    if (ImGui::Button(u8"重置布局")) {
        // 移除根节点，布局会自动重建
        ImGui::DockBuilderRemoveNode(dockspaceID);
    }
}
ImGui::End();

// 消息框
if (ImGui::Begin(UI_MESSAGE_BOX)) {
    ImGui::LabelText("label", "text");
    ImGui::Button("button");
}
ImGui::End();

// 日志框
if (ImGui::Begin(UI_LOG_BOX)) {
    ImGui::LabelText("label", "text");
    ImGui::Button("button");
}
ImGui::End();

// 观察窗口，背景设置透明，窗口后面就能进行本地 API 的绘制
// 压入样式设置
ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);            // 无边框
ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // 无边界
ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);              // 无圆角
// ImGui::SetNextWindowBgAlpha(0.0f); // 窗口 alpha 为 0，同样可以不显示背景

if (ImGui::Begin(UI_VIEW_BOX, 0, ImGuiWindowFlags_NoBackground)) { // 无背景窗口
    // 获取窗口坐标
    ImVec2 pos  = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::Text("position: %0.2f, %0.2f", pos.x, pos.y);
    ImGui::Text("size: %0.2f, %0.2f", size.x, size.y);

    // 记录下视口位置给本地 API 使用
}
    // Core::Instance().ImGuiAll();

ImGui::End();
ImGui::PopStyleVar(3); // 弹出样式设置

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
