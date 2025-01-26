//
// Created by TrifingZW on 24-12-30.
//

#include <glad/glad.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "e_terrain.h"
#include "core/core.h"
#include "core/helpers/assets_helper.h"
#include "core/icon/IconsFontAwesome6.h"
#include "core/icon/IconsMaterialDesign.h"
#include "platform/android/android.h"
#include "platform/android/android_out.h"

float GetSystemDPI(const android_app* app)
{
    JNIEnv* java_env = nullptr;
    JavaVM* java_vm = app->activity->vm;

    // 检查线程是否已经附加
    if (java_vm->GetEnv(reinterpret_cast<void **>(&java_env), JNI_VERSION_1_6) != JNI_OK)
        java_vm->AttachCurrentThread(&java_env, nullptr);

    jclass native_activity_clazz = java_env->GetObjectClass(app->activity->clazz);
    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "getDpiSize", "()F");

    if (method_id == nullptr)
        return 4.0f;

    // 调用 getDpiSize() 获取 DPI
    jfloat dpi = java_env->CallFloatMethod(app->activity->clazz, method_id);

    // 分离线程并恢复原始状态
    if (java_vm->GetEnv(reinterpret_cast<void **>(&java_env), JNI_VERSION_1_6) != JNI_OK)
        java_vm->DetachCurrentThread();

    return dpi;
}


// 辅助函数的前向声明
static void Init(android_app* app);
static void InitEGL();
static void InitImGui();
static void RenderImGui();
static void RenderOpenGl();
static void Shutdown();
static void MainLoopStep();
static int ShowSoftKeyboardInput();
static int PollUnicodeChars();

void handleAppCmd(android_app* app, const int32_t appCmd)
{
    switch (appCmd)
    {
        case APP_CMD_SAVE_STATE:
            aout << "APPCMD 保存状态" << std::endl;
            break;
        case APP_CMD_INIT_WINDOW:
            aout << "APPCMD 初始化窗口" << std::endl;
            Init(app);
            break;
        case APP_CMD_TERM_WINDOW:
            aout << "APPCMD 关闭窗口" << std::endl;
            Shutdown();
            break;
        case APP_CMD_GAINED_FOCUS:
            aout << "APPCMD 获得焦点" << std::endl;
            break;
        case APP_CMD_LOST_FOCUS:
            aout << "APPCMD 丢失焦点" << std::endl;
            break;
        default:
            aout << "APPCMD 无法匹配" << std::endl;
            break;
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static int32_t handleInputEvent(android_app* app, AInputEvent* inputEvent)
{
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

void android_main(android_app* app)
{
    app->onAppCmd = handleAppCmd;
    app->onInputEvent = handleInputEvent;

    while (true)
    {
        int out_events;
        android_poll_source* out_data;

        // 轮询所有事件。如果应用程序不可见，则此循环将阻塞，直到 g_Initialized == true。
        while (ALooper_pollOnce(Android::g_Initialized ? 0 : -1, nullptr, &out_events, reinterpret_cast<void **>(&out_data)) >= 0)
        {
            // 处理一个事件
            if (out_data != nullptr)
                out_data->process(app, out_data);

            // 通过从无限循环中返回来退出应用。
            if (app->destroyRequested != 0)
            {
                // 在处理应用命令 APP_CMD_TERM_WINDOW 时，shutdown() 应该已经被调用了。但我们在这里为了保险起见。
                if (!Android::g_Initialized)
                    Shutdown();

                return;
            }
        }

        // 启动一个新的帧
        if (Android::g_Initialized)
            MainLoopStep();
    }
}

void Init(android_app* app)
{
    if (Android::g_Initialized)
        return;

    // 增加 ANativeWindow 对象的引用计数，确保这个窗口资源不会被意外释放
    Android::g_App = app;
    ANativeWindow_acquire(Android::g_App->window);

    // 初始化EGL
    InitEGL();

    // 初始化 GLAD
    if (!gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(eglGetProcAddress)))
        std::cerr << "Failed to initialize GLAD!" << std::endl;

    aout << glGetString(GL_VERSION) << std::endl;

    // 将 .ini 配置文件的加载/保存重定向位置到 Android 内部储存
    // 确保在我们使用 Dear ImGui 时“g_IniFilename”仍然存在。
    Android::g_IniFilename = std::string(app->activity->internalDataPath) + "/imgui.ini";

    // 初始化 ImGui 和 Core
    InitImGui();
    Core::Instance().InitTree();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Android::g_Initialized = true;
}

// 这主要是 EGL 的样板代码...
void InitEGL()
{
    Android::g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (Android::g_EglDisplay == EGL_NO_DISPLAY)
        __android_log_print(ANDROID_LOG_ERROR, Android::g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

    if (eglInitialize(Android::g_EglDisplay, nullptr, nullptr) != EGL_TRUE)
        __android_log_print(ANDROID_LOG_ERROR, Android::g_LogTag, "%s", "eglInitialize() returned with an error");

    const EGLint egl_attributes[] = {
        EGL_BLUE_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_RED_SIZE,
        8,
        EGL_DEPTH_SIZE,
        24,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_NONE
    };
    EGLint num_configs = 0;
    if (eglChooseConfig(Android::g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
        __android_log_print(ANDROID_LOG_ERROR, Android::g_LogTag, "%s", "eglChooseConfig() returned with an error");
    if (num_configs == 0)
        __android_log_print(ANDROID_LOG_ERROR, Android::g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");

    // 获取第一个匹配的配置
    EGLConfig egl_config;
    eglChooseConfig(Android::g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
    EGLint egl_format;
    eglGetConfigAttrib(Android::g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
    ANativeWindow_setBuffersGeometry(Android::g_App->window, 0, 0, egl_format);

    constexpr EGLint egl_context_attributes[] = {
        EGL_CONTEXT_MAJOR_VERSION,
        3,
        // 主版本号
        EGL_CONTEXT_MINOR_VERSION,
        2,
        // 次版本号
        EGL_NONE
    };

    Android::g_EglContext = eglCreateContext(Android::g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

    if (Android::g_EglContext == EGL_NO_CONTEXT)
        __android_log_print(ANDROID_LOG_ERROR, Android::g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");

    Android::g_EglSurface = eglCreateWindowSurface(Android::g_EglDisplay, egl_config, Android::g_App->window, nullptr);
    eglMakeCurrent(Android::g_EglDisplay, Android::g_EglSurface, Android::g_EglSurface, Android::g_EglContext);
}

void InitImGui()
{
    // 设置 Dear ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = Android::g_IniFilename.c_str();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 启用停靠功能

    // 设置dpi为系统dpi
    ETerrain::DpiScale = GetSystemDPI(Android::g_App);
    ETerrain::FontScale = 16 * ETerrain::DpiScale;
    ImGui::GetStyle().ScaleAllSizes(ETerrain::DpiScale);

    // 设置字体
    void* harmony = nullptr;
    void* material = nullptr;
    void* far = nullptr;
    void* fas = nullptr;
    const size_t harmony_len = AssetsHelper::GetAssetData("fonts/HarmonyOS_Sans_SC_Bold.ttf", &harmony);
    const size_t material_len = AssetsHelper::GetAssetData(FONT_ICON_FILE_NAME_MD, &material);
    const size_t far_len = AssetsHelper::GetAssetData(FONT_ICON_FILE_NAME_FAR, &far);
    const size_t fas_len = AssetsHelper::GetAssetData(FONT_ICON_FILE_NAME_FAS, &fas);
    io.Fonts->AddFontFromMemoryTTF(
        harmony,
        static_cast<int>(harmony_len),
        16 * ETerrain::DpiScale,
        nullptr,
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon()
    );
    static ImWchar material_ranges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
    static ImWchar awesome6_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig cfg;
    cfg.MergeMode = true;
    cfg.PixelSnapH = true;
    cfg.OversampleH = 1;
    cfg.OversampleV = 1;
    cfg.GlyphMinAdvanceX = ETerrain::FontScale;
    io.Fonts->AddFontFromMemoryTTF(material, static_cast<int>(material_len), ETerrain::FontScale, &cfg, material_ranges);
    io.Fonts->AddFontFromMemoryTTF(far, static_cast<int>(far_len), ETerrain::FontScale, &cfg, awesome6_ranges);
    io.Fonts->AddFontFromMemoryTTF(fas, static_cast<int>(fas_len), ETerrain::FontScale, &cfg, awesome6_ranges);

    // 设置平台/渲染器后端
    ImGui_ImplAndroid_Init(Android::g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
}

void MainLoopStep()
{
    const ImGuiIO& io = ImGui::GetIO();
    if (Android::g_EglDisplay == EGL_NO_DISPLAY)
        return;

    // Poll Unicode characters via JNI
    // FIXME: do not call this every frame because of JNI overhead
    PollUnicodeChars();

    // Open on-screen (soft) input if requested by Dear ImGui
    static bool WantTextInputLast = false;
    if (io.WantTextInput && !WantTextInputLast)
        ShowSoftKeyboardInput();
    WantTextInputLast = io.WantTextInput;

    // 渲染 OpenGL 和 ImGui 内容
    RenderOpenGl();
    RenderImGui();

    eglSwapBuffers(Android::g_EglDisplay, Android::g_EglSurface);

    if (const GLenum status = glGetError(); status != GL_NO_ERROR)
        std::cerr << "GL_ERROR:" << status << std::endl;
}

// 渲染 ImGui 内容
void RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
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

// 清理
void Shutdown()
{
    if (!Android::g_Initialized)
        return;

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    if (Android::g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(Android::g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (Android::g_EglContext != EGL_NO_CONTEXT)
            eglDestroyContext(Android::g_EglDisplay, Android::g_EglContext);

        if (Android::g_EglSurface != EGL_NO_SURFACE)
            eglDestroySurface(Android::g_EglDisplay, Android::g_EglSurface);

        eglTerminate(Android::g_EglDisplay);
    }

    Android::g_EglDisplay = EGL_NO_DISPLAY;
    Android::g_EglContext = EGL_NO_CONTEXT;
    Android::g_EglSurface = EGL_NO_SURFACE;
    ANativeWindow_release(Android::g_App->window);

    Android::g_Initialized = false;
}

// Helper functions
// Unfortunately, there is no way to show the on-screen input from native code.
// Therefore, we call ShowSoftKeyboardInput() of the main activity implemented in MainActivity.kt via JNI.
static int ShowSoftKeyboardInput()
{
    JavaVM* java_vm = Android::g_App->activity->vm;
    JNIEnv* java_env = nullptr;

    jint jni_return = java_vm->GetEnv(reinterpret_cast<void **>(&java_env), JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, nullptr);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(Android::g_App->activity->clazz);
    if (native_activity_clazz == nullptr)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "showSoftInput", "()V");
    if (method_id == nullptr)
        return -4;

    java_env->CallVoidMethod(Android::g_App->activity->clazz, method_id);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}

// Unfortunately, the native KeyEvent implementation has no getUnicodeChar() function.
// Therefore, we implement the processing of KeyEvents in MainActivity.kt and poll
// the resulting Unicode characters here via JNI and send them to Dear ImGui.
static int PollUnicodeChars()
{
    JavaVM* java_vm = Android::g_App->activity->vm;
    JNIEnv* java_env = nullptr;

    jint jni_return = java_vm->GetEnv(reinterpret_cast<void **>(&java_env), JNI_VERSION_1_6);
    if (jni_return == JNI_ERR)
        return -1;

    jni_return = java_vm->AttachCurrentThread(&java_env, nullptr);
    if (jni_return != JNI_OK)
        return -2;

    jclass native_activity_clazz = java_env->GetObjectClass(Android::g_App->activity->clazz);
    if (native_activity_clazz == nullptr)
        return -3;

    jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "pollUnicodeChar", "()I");
    if (method_id == nullptr)
        return -4;

    // Send the actual characters to Dear ImGui
    ImGuiIO& io = ImGui::GetIO();
    jint unicode_character;
    while ((unicode_character = java_env->CallIntMethod(Android::g_App->activity->clazz, method_id)) != 0)
        io.AddInputCharacter(unicode_character);

    jni_return = java_vm->DetachCurrentThread();
    if (jni_return != JNI_OK)
        return -5;

    return 0;
}
