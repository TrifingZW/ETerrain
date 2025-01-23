cmake_minimum_required(VERSION 3.30.5)
set(PLATFORM "windows")

# 为平台设置工具链和参数并定义宏
if (PLATFORM STREQUAL "windows")
    add_definitions(-DPLATFORM_WINDOWS)
elseif (PLATFORM STREQUAL "android")
    add_definitions(-DPLATFORM_ANDROID)
    set(ANDROID_NDK "C:/Users/TrifingZW/AppData/Local/Android/Sdk/ndk/27.2.12479018")
    if (NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
        set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")
        set(ANDROID_ABI arm64-v8a)
        set(ANDROID_NATIVE_API_LEVEL 24)
        set(ANDROID_STL c++_static)
    endif ()
endif ()

project(ETerrain)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# 设置二进制文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
# 导入方法
include(method.cmake)

# 定义并添加源文件
set(SOURCE_FILES
        # 渲染器后端
        core/graphics/graphics_device.cpp
        core/graphics/graphics_device.h
        core/graphics/sprite_batch.cpp
        core/graphics/sprite_batch.h
        core/graphics/buffer_manager.cpp
        core/graphics/buffer_manager.h
        core/graphics/render_target.cpp
        core/graphics/render_target.h
        core/graphics/graphics_structure.h
        core/graphics/graphics_enum.h
        core/graphics/sampler_state.cpp
        core/graphics/sampler_state.h
        core/graphics/graphics_resource.cpp
        core/graphics/graphics_resource.h
        core/graphics/graphics_helper.h
        core/graphics/texture_collection.cpp
        core/graphics/texture_collection.h
        core/graphics/sampler_state_collection.cpp
        core/graphics/sampler_state_collection.h
        core/graphics/vertex_declaration.h
        core/graphics/vertex_type.h

        # 核心
        core/core.cpp
        core/core.h

        # 小帮手
        core/helpers/assets_helper.cpp
        core/helpers/assets_helper.h
        core/helpers/imgui_helper.cpp
        core/helpers/imgui_helper.h

        # 数学
        core/math/math_defs.h
        core/math/math_funcs.h
        core/math/transform_2d.cpp
        core/math/transform_2d.h
        core/math/transform_3d.cpp
        core/math/transform_3d.h
        core/math/vector2.cpp
        core/math/vector2.h
        core/math/vector2i.cpp
        core/math/vector2i.h

        # 场景
        scene/node.cpp
        scene/node.h
        scene/2d/node_2d.cpp
        scene/2d/node_2d.h
        scene/2d/camera_2d.cpp
        scene/2d/camera_2d.h
        scene/3d/node_3d.cpp
        scene/3d/node_3d.h
        scene/3d/camera_3d.cpp
        scene/3d/camera_3d.h
        scene/resources/shader.cpp
        scene/resources/shader.h
        scene/resources/texture_2d.cpp
        scene/resources/texture_2d.h

        # 命名空间
        e_terrain.h

        # 编辑器
        editor/editor.cpp
        editor/editor.h
        editor/load_resources.cpp
        editor/load_resources.h
        editor/panel/game_panel_2d.cpp
        editor/panel/game_panel_2d.h
        editor/panel/game_panel_3D.cpp
        editor/panel/game_panel_3D.h
        editor/panel/hex_manager.cpp
        editor/panel/hex_manager.h
        editor/panel/hex_panel.cpp
        editor/panel/hex_panel.h
        editor/parser/map_config_parser.cpp
        editor/parser/map_config_parser.h
        editor/parser/terrain_config_parser.cpp
        editor/parser/terrain_config_parser.h
        editor/parser/resource_texture_parser.cpp
        editor/parser/resource_texture_parser.h
        editor/parser/bin_parser.cpp
        editor/parser/bin_parser.h
        editor/parser/btl_structure.h
        editor/world/land_unit.cpp
        editor/world/land_unit.h

        # 定义
        core/typedefs.h
)
if (PLATFORM STREQUAL "windows")
    list(APPEND SOURCE_FILES main.cpp)
    add_executable(ETerrain ${SOURCE_FILES})
elseif (PLATFORM STREQUAL "android")
    list(APPEND SOURCE_FILES android_main.cpp)
    add_library(ETerrain SHARED ${SOURCE_FILES})
endif ()

# 设置核心件路径
set(CORE_DIR ${CMAKE_SOURCE_DIR}/core)
set(SCENE_DIR ${CMAKE_SOURCE_DIR}/scene)
set(EDITOR_DIR ${CMAKE_SOURCE_DIR}/editor)
# 设置模块路径
set(MODULE_DIR ${CMAKE_SOURCE_DIR}/modules)
# 设置第三方库路径
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
# 设置平台路径
set(PLATFORM_DIR ${CMAKE_SOURCE_DIR}/platform)

# 包含生成的头文件路径，只在构建时包含
target_include_directories(ETerrain PRIVATE ${CMAKE_BINARY_DIR})
# 导入核心和第三方库文件夹
target_include_directories(ETerrain PRIVATE ${CMAKE_SOURCE_DIR})
#[[target_include_directories(ETerrain PRIVATE ${CORE_DIR})
target_include_directories(ETerrain PRIVATE ${SCENE_DIR})
target_include_directories(ETerrain PRIVATE ${EDITOR_DIR})]]
#include_directories(${THIRDPARTY_DIR})

# 导入glad
add_library(glad STATIC thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ${THIRDPARTY_DIR}/glad/include)
target_link_libraries(ETerrain PRIVATE glad)

# 导入glm
set(GLM_BUILD_LIBRARY ON)
add_definitions(-DGLM_BUILD_LIBRARY)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(${THIRDPARTY_DIR}/glm)
target_link_libraries(ETerrain PRIVATE glm::glm)

LoadPlatformDependencies()
LoadModules()
LoadImGui()
LoadPugixml()
LoadStb()
LoadThorvg()
LoadShader(game)
