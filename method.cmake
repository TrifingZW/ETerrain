function(LoadPlatformDependencies)
    add_library(platform STATIC)
    set(PLATFORM_SOURCES)

    # 当平台为 Windows 时，导入 glfw，当平台为 Android 时，导入 Android 相关库和文件夹并链接
    if (PLATFORM STREQUAL "windows")
        set(DIR ${PLATFORM_DIR}/windows)
        list(APPEND PLATFORM_SOURCES
                ${DIR}/placeholder_class.cpp
        )

        include_directories(${THIRDPARTY_DIR}/glfw/include)
        target_link_libraries(ETerrain PRIVATE ${THIRDPARTY_DIR}/glfw/lib-vc2022/glfw3.lib)

    elseif (PLATFORM STREQUAL "android")
        set(DIR ${PLATFORM_DIR}/android)
        list(APPEND PLATFORM_SOURCES
                ${DIR}/android_out.cpp
                ${DIR}/android.cpp
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

function(LoadThorvg)

    add_definitions(-DTVG_STATIC)

    add_library(thorvg STATIC)
    set(DIR ${THIRDPARTY_DIR}/thorvg)

    target_include_directories(ETerrain PRIVATE ${DIR}/inc)
    target_include_directories(thorvg PRIVATE ${DIR}/inc)
    target_include_directories(thorvg PRIVATE ${DIR}/src/common)
    target_include_directories(thorvg PRIVATE ${DIR}/src/loaders/svg)
    target_include_directories(thorvg PRIVATE ${DIR}/src/renderer)
    target_include_directories(thorvg PRIVATE ${DIR}/src/renderer/sw_engine)
    target_include_directories(thorvg PRIVATE ${DIR}/src/loaders/raw)

    set(MODULE_SOURCES
            # common
            ${DIR}/src/common/tvgCompressor.cpp
            ${DIR}/src/common/tvgMath.cpp
            ${DIR}/src/common/tvgStr.cpp

            # SVG parser
            ${DIR}/src/loaders/svg/tvgSvgCssStyle.cpp
            ${DIR}/src/loaders/svg/tvgSvgLoader.cpp
            ${DIR}/src/loaders/svg/tvgSvgPath.cpp
            ${DIR}/src/loaders/svg/tvgSvgSceneBuilder.cpp
            ${DIR}/src/loaders/svg/tvgSvgUtil.cpp
            ${DIR}/src/loaders/svg/tvgXmlParser.cpp
            ${DIR}/src/loaders/raw/tvgRawLoader.cpp

            # renderer common
            ${DIR}/src/renderer/tvgAccessor.cpp
            #  ${DIR}/src/renderer/tvgAnimation.cpp
            ${DIR}/src/renderer/tvgCanvas.cpp
            ${DIR}/src/renderer/tvgFill.cpp
            #  ${DIR}/"src/renderer/tvgGlCanvas.cpp
            ${DIR}/src/renderer/tvgInitializer.cpp
            ${DIR}/src/renderer/tvgLoader.cpp
            ${DIR}/src/renderer/tvgPaint.cpp
            ${DIR}/src/renderer/tvgPicture.cpp
            ${DIR}/src/renderer/tvgRender.cpp
            #  ${DIR}/"src/renderer/tvgSaver.cpp
            ${DIR}/src/renderer/tvgScene.cpp
            ${DIR}/src/renderer/tvgShape.cpp
            ${DIR}/src/renderer/tvgSwCanvas.cpp
            ${DIR}/src/renderer/tvgTaskScheduler.cpp
            ${DIR}/src/renderer/tvgText.cpp
            #  ${DIR}/src/renderer/tvgWgCanvas.cpp

            # renderer sw_engine
            ${DIR}/src/renderer/sw_engine/tvgSwFill.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwImage.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwMath.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwMemPool.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwPostEffect.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwRaster.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwRenderer.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwRle.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwShape.cpp
            ${DIR}/src/renderer/sw_engine/tvgSwStroke.cpp
    )

    target_sources(thorvg PRIVATE ${MODULE_SOURCES})
    target_link_libraries(ETerrain PRIVATE thorvg)

endfunction()

function(LoadShader ShaderName)
    # 读取顶点着色器和片段着色器内容
    set(SHADER_DIR "misc/glsl/")
    set(VERTEX_SHADER "${SHADER_DIR}${ShaderName}.vert")
    set(FRAGMENT_SHADER "${SHADER_DIR}${ShaderName}.frag")

    # 设置依赖关系，确保 GLSL 文件更改时强制更新
    set(SHADER_DEPENDENCIES ${VERTEX_SHADER} ${FRAGMENT_SHADER})

    # 生成 shader 头文件（如果你确实需要通过模板生成的话）
    configure_file(misc/glsl/shader_template.h.in ${ShaderName}_shader.h)

    # 添加 custom command 重新生成 header 文件
    add_custom_command(
            OUTPUT ${ShaderName}_shader.h
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VERTEX_SHADER} ${FRAGMENT_SHADER} ${ShaderName}_shader.h
            DEPENDS ${SHADER_DEPENDENCIES}
            COMMENT "Updating shader header for ${ShaderName}"
    )

endfunction()

# 自动加载目录中的所有着色器，并生成头文件
function(GenerateShaderHeaders)
    # 设置 GLSL 文件夹路径
    set(SHADER_DIR "misc/glsl/")

    # 获取所有 .vert 和 .frag 文件
    file(GLOB_RECURSE VERTEX_SHADERS "${SHADER_DIR}*.vert")
    file(GLOB_RECURSE FRAGMENT_SHADERS "${SHADER_DIR}*.frag")

    # 合并所有顶点着色器和片段着色器
    list(APPEND SHADERS ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})

    # 遍历每个着色器文件并生成对应的头文件
    foreach (SHADER_FILE IN LISTS SHADERS)
        # 获取不带扩展名的文件名 (即 ShaderName)
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)

        # 设置输出头文件路径
        set(OUTPUT_HEADER "${CMAKE_BINARY_DIR}/${SHADER_NAME}_shader.h")

        # 读取顶点着色器和片段着色器内容
        file(READ ${SHADER_DIR}${SHADER_NAME}.vert SHADER_VSH_CONTENT)
        file(READ ${SHADER_DIR}${SHADER_NAME}.frag SHADER_FSH_CONTENT)

        # 通过模板配置生成最终头文件
        configure_file(misc/glsl/shader_template.h.in ${SHADER_NAME}_shader.h)

        # 添加 custom command 以便着色器文件更新时重新生成头文件
        add_custom_command(
                OUTPUT ${OUTPUT_HEADER}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/misc/glsl/shader_template.h.in"
                "${OUTPUT_HEADER}"
                DEPENDS ${SHADER_FILE}
                COMMENT "Generating shader header for ${SHADER_NAME}"
        )
    endforeach ()
endfunction()




