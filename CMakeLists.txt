cmake_minimum_required(VERSION 3.29)
project(ETerrain)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
# 设置二进制文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
# 导入方法
include(method.cmake)

#add_definitions(-DBUILD_TIME)
set(PLATFORM "windows")

# 包含生成的头文件路径，只在构建时包含
include_directories(${CMAKE_BINARY_DIR})

# 添加文件
add_executable(ETerrain
        main.cpp
        misc/fonts/font.h
        core/Core.cpp
        core/Core.h
        core/editor.cpp
        core/editor.h
        core/Node.cpp
        core/Node.h
        core/scene/panel/game_panel_3D.cpp
        core/scene/panel/game_panel_3D.h
        core/nodes/load_resources.cpp
        core/nodes/load_resources.h
        core/btl_structure.h
        core/parser/map_config_parser.cpp
        core/parser/map_config_parser.h
        core/parser/terrain_config_parser.cpp
        core/parser/terrain_config_parser.h
        core/parser/resource_texture_parser.cpp
        core/parser/resource_texture_parser.h
        core/scene/resources/shader.cpp
        core/scene/resources/shader.h
        core/scene/2d/camera_2d.cpp
        core/scene/2d/camera_2d.h
        core/scene/3d/camera_3d.cpp
        core/scene/3d/camera_3d.h
        core/math/transform_2d.cpp
        core/math/transform_2d.h
        core/math/transform_3d.cpp
        core/math/transform_3d.h
        core/scene/panel/game_panel_2d.cpp
        core/scene/panel/game_panel_2d.h
        core/scene/resources/texture_2d.cpp
        core/scene/resources/texture_2d.h
        core/scene/3d/node_3d.cpp
        core/scene/3d/node_3d.h
        core/scene/2d/node_2d.cpp
        core/scene/2d/node_2d.h
        core/graphics/sprite_batch.cpp
        core/graphics/sprite_batch.h
        core/graphics/graphics_device.cpp
        core/graphics/graphics_device.h
        core/graphics/render_target.cpp
        core/graphics/render_target.h
        core/graphics/sprite_sortmode.h
        core/graphics/blend_state.cpp
        core/graphics/blend_state.h
        core/graphics/sampler_state.cpp
        core/graphics/sampler_state.h
        core/graphics/depthstencil_state.cpp
        core/graphics/depthstencil_state.h
        core/graphics/rasterizer_state.cpp
        core/graphics/rasterizer_state.h
)

# 设置核心件路径
set(CORE_DIR ${CMAKE_SOURCE_DIR}/core)
# 设置模块路径
set(MODULE_DIR ${CMAKE_SOURCE_DIR}/modules)
# 设置第三方库路径
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# 导入核心和第三方库文件夹
include_directories(${CORE_DIR})
#include_directories(${THIRDPARTY_DIR})
# 导入glad
include_directories(${THIRDPARTY_DIR}/glad/include)
add_library(glad STATIC thirdparty/glad/src/glad.c)
target_link_libraries(ETerrain glad)
# 导入glfw
include_directories(${THIRDPARTY_DIR}/glfw/include)
target_link_libraries(ETerrain ${THIRDPARTY_DIR}/glfw/lib-vc2022/glfw3.lib)
# 导入glm
set(GLM_BUILD_LIBRARY ON)
add_definitions(-DGLM_BUILD_LIBRARY)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(${THIRDPARTY_DIR}/glm)
target_link_libraries(ETerrain glm::glm)


LoadModules()
LoadImGui()
LoadPugixml()
LoadStb()
LoadFont()

