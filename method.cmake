function(LoadPlatformDependencies)
    add_library(platform STATIC)
    set(PLATFORM_SOURCES)

    # 当平台为 Windows 时，导入 glfw，当平台为 Android 时，导入 Android 相关库和文件夹并链接
    if (PLATFORM STREQUAL "windows")
        set(DIR ${PLATFORM_DIR}/windows)
        list(APPEND PLATFORM_SOURCES
                ${DIR}/placeholder_class.cpp
                ${DIR}/placeholder_class.h
        )

        include_directories(${THIRDPARTY_DIR}/glfw/include)
        target_link_libraries(ETerrain PRIVATE ${THIRDPARTY_DIR}/glfw/lib-vc2022/glfw3.lib)

    elseif (PLATFORM STREQUAL "android")
        set(DIR ${PLATFORM_DIR}/android)
        list(APPEND PLATFORM_SOURCES
                ${DIR}/android_out.cpp
                ${DIR}/android_out.h
                ${DIR}/android.cpp
                ${DIR}/android.h
        )

        include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
        target_sources(ETerrain PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
        target_link_libraries(ETerrain PRIVATE
                android
                EGL
                GLESv3
                log
        )
    endif ()

    target_sources(platform PRIVATE ${PLATFORM_SOURCES})
    target_link_libraries(ETerrain PRIVATE platform)
endfunction()

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
    target_link_libraries(ETerrain PRIVATE ${ALL_MODULE_LIBS})

endfunction()

function(LoadImGui)

    add_library(imgui STATIC)
    set(DIR ${THIRDPARTY_DIR}/imgui)

    # 导入头文件所在目录
    target_include_directories(imgui PUBLIC ${DIR})
    target_include_directories(imgui PUBLIC ${DIR}/backends)

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
        target_compile_definitions(ETerrain PRIVATE IMGUI_IMPL_OPENGL_ES3)
        list(APPEND MODULE_SOURCES ${DIR}/backends/imgui_impl_android.cpp)
    endif ()

    target_sources(imgui PRIVATE ${MODULE_SOURCES})
    target_link_libraries(ETerrain PRIVATE imgui)

endfunction()

function(LoadPugixml)

    add_library(pugixml STATIC)
    set(DIR ${THIRDPARTY_DIR}/pugixml/src)

    # 导入头文件所在目录
    target_include_directories(ETerrain PRIVATE ${DIR})

    # 源文件列表
    set(MODULE_SOURCES ${DIR}/pugixml.cpp)

    target_sources(pugixml PRIVATE ${MODULE_SOURCES})
    target_link_libraries(ETerrain PRIVATE pugixml)

endfunction()

function(LoadStb)

    set(DIR ${THIRDPARTY_DIR}/stb)
    target_include_directories(ETerrain PRIVATE ${DIR})
    #    add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

endfunction()

function(LoadFont)

    # 定义字体文件路径
    set(FONT_DIR ${CMAKE_SOURCE_DIR}/misc/fonts)
    target_include_directories(ETerrain PRIVATE ${FONT_DIR})
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

function(LoadShader ShaderName)
    # 读取顶点着色器和片段着色器内容
    file(READ "misc/glsl/${ShaderName}.vsh" SHADER_VSH_CONTENT)
    file(READ "misc/glsl/${ShaderName}.fsh" SHADER_FSH_CONTENT)

    # 设置依赖关系，确保当 GLSL 文件更改时强制更新
    set(SHADER_DEPENDENCIES "misc/glsl/${ShaderName}.vsh" "misc/glsl/${ShaderName}.fsh")

    # 生成 shader 头文件
    configure_file(misc/glsl/shader_template.h.in ${ShaderName}_shader.h)

    # 设置 shader 文件为依赖，确保在文件修改时重新生成
    add_custom_command(
            OUTPUT ${ShaderName}_shader.h
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "misc/glsl/${ShaderName}.vsh" "misc/glsl/${ShaderName}.fsh" ${ShaderName}_shader.h
            DEPENDS ${SHADER_DEPENDENCIES}
            COMMENT "Updating shader header for ${ShaderName}"
    )

endfunction()


