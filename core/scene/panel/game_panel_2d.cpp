//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>

#include "editor.h"

void GamePanel2D::Init() {
    NewFramebuffer();
    InitOpenGlResources();
}

void GamePanel2D::Ready() {
}

void GamePanel2D::Rendering(const SpriteBatch &spriteBatch) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture2D texture2d = Editor::loadResources->plantResourceTextureParser.Texture2D;
    shader.Use();
    glm::mat4 model(1.0f);
    model = translate(model, glm::vec3(glm::vec2(1.0f), 0.0f));
    model = scale(model, glm::vec3(glm::vec2(239, 136), 1.0f));
    glm::mat4 projection = camera2d->GetProjectionMatrix();
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2d.Id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GamePanel2D::Gui() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框


    ImGui::Begin("Game2D");

    const ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (static_cast<int>(panelSize.x) != width || static_cast<int>(panelSize.y) != height) {
        width = static_cast<int>(panelSize.x);
        height = static_cast<int>(panelSize.y);
        NewFramebuffer();
    }

    ImGui::Image(
        static_cast<ImTextureID>(static_cast<intptr_t>(textureBuffer)),
        panelSize,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void GamePanel2D::NewFramebuffer() {
    // 设置相机视口大小
    camera2d->SetViewportSize(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(5.0f);
    // 生成新缓冲前先删除
    if (framebuffer) {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    if (textureBuffer) {
        glDeleteTextures(1, &textureBuffer);
        textureBuffer = 0;
    }

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB,GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, textureBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GamePanel2D::InitOpenGlResources() {
    const std::vector<float> vertices = Editor::loadResources->plantResourceTextureParser.GetVertices("desert_4.png");
    // @formatter:off
    constexpr unsigned int indices[] = {
        2, 1, 3, // 第一个三角形
        1, 0, 3  // 第二个三角形
    };
    // @formatter:on

    // 创建 VAO VBO EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 绑定 VBO 并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices.size()) * 4, vertices.data(), GL_STATIC_DRAW);
    // 绑定 EBO 并上传数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
    // 属性
    glVertexAttribPointer(0, 4,GL_FLOAT,GL_FALSE, 4 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // 切换回屏幕
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    shader = Shader();
}
