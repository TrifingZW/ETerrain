function(LoadModules)

    # 用于存储所有模块生成的目标
    set(ALL_MODULE_LIBS)
    # 收集所有模块的构建脚本
    file(GLOB MODULE_CMAKE_FILES "${MODULE_DIR}/*/module.cmake")

    # 遍历每个模块
    foreach (MODULE_CMAKE_FILE ${MODULE_CMAKE_FILES})
        # 获取模块目录名作为模块名
        get_filename_component(MODULE_DIR ${MODULE_CMAKE_FILE} DIRECTORY)
        get_filename_component(MODULE_NAME ${MODULE_DIR} NAME)

        # 设置模块构建的专属变量和环境
        set(MODULE_INCLUDE_DIR ${MODULE_DIR})
        set(MODULE_SOURCE_DIR ${MODULE_DIR})

        # 定义模块库
        add_library(${MODULE_NAME} STATIC)

        # 加载模块的构建脚本
        include(${MODULE_CMAKE_FILE})

        # 将模块库添加到全局模块列表
        list(APPEND ALL_MODULE_LIBS ${MODULE_NAME})
    endforeach ()

    # 将模块库链接到主项目
    target_link_libraries(ETerrain ${ALL_MODULE_LIBS})

endfunction()

function(LoadImGui)

    add_library(imgui STATIC)
    set(DIR ${THIRDPARTY_DIR}/imgui)

    # 导入头文件所在目录
    include_directories(${DIR})
    include_directories(${DIR}/backends)

    # 源文件列表
    set(MODULE_SOURCES
            ${DIR}/imgui.cpp
            ${DIR}/imgui_demo.cpp
            ${DIR}/imgui_draw.cpp
            ${DIR}/imgui_tables.cpp
            ${DIR}/imgui_widgets.cpp
            ${DIR}/backends/imgui_impl_opengl3.cpp
    )

    # 判断平台
    if (PLATFORM STREQUAL "windows")

        list(APPEND MODULE_SOURCES ${DIR}/backends/imgui_impl_glfw.cpp)

    elseif (PLATFORM STREQUAL "android")

    endif ()

    target_sources(imgui PRIVATE ${MODULE_SOURCES})
    target_link_libraries(ETerrain imgui)

endfunction()

function(LoadPugixml)

    add_library(pugixml STATIC)
    set(DIR ${THIRDPARTY_DIR}/pugixml/src)

    # 导入头文件所在目录
    include_directories(${DIR})

    # 源文件列表
    set(MODULE_SOURCES ${DIR}/pugixml.cpp)

    target_sources(pugixml PRIVATE ${MODULE_SOURCES})
    target_link_libraries(ETerrain pugixml)

endfunction()

function(LoadStb)

    set(DIR ${THIRDPARTY_DIR}/stb)
    include_directories(${DIR})
#    add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

endfunction()

function(LoadFont)

    # 定义字体文件路径
    set(FONT_DIR ${CMAKE_SOURCE_DIR}/misc/fonts)
    include_directories(${FONT_DIR})
    set(FONT_SOURCE ${FONT_DIR}/HarmonyOS_Sans_SC_Bold.ttf)
    set(FONT_HEADER ${CMAKE_BINARY_DIR}/harmony_os_sans_sc_bold.h)

    add_custom_command(
            DEPENDS ${FONT_SOURCE}
            OUTPUT ${FONT_HEADER}
            COMMAND python ${FONT_DIR}/generate_font_array.py ${FONT_SOURCE} ${FONT_HEADER}
            COMMENT "Generating HarmonyOS_Sans_SC_Bold.ttf from harmony_os_sans_sc_bold.h"
    )

    # 添加自定义目标，确保字节数组头文件先生成
    add_custom_target(EmbedFont DEPENDS ${FONT_HEADER})
    add_dependencies(ETerrain EmbedFont)

endfunction()
