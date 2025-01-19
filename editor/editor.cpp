//
// Created by TrifingZW on 24-11-17.
//

#include "editor.h"

LoadResources* Editor::loadResources = nullptr;

Editor::Editor()
{
    /*auto* panelHex = new HexPanel();
        hexPanel = panelHex;
        AddChild(std::unique_ptr<Node>(panelHex));*/

    auto* panel2d = new GamePanel2D();
    gamePanel2d = panel2d;
    AddChild(panel2d);

    auto* load_resources = new LoadResources();
    loadResources = load_resources;
    AddChild(load_resources);
}

Editor::~Editor()
{
    /*delete hexPanel;
    delete gamePanel2d;
    delete gamePanel3d;
    delete loadResources;*/
}

void Editor::Gui()
{
    ImGuiDockNodeFlags docks_pace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // 全屏
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    if (docks_pace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    dockSpaceId = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), docks_pace_flags);

    if (ImGui::BeginMenuBar())
    {
        //增加主题切换
        if (ImGui::BeginMenu("主题（Other）"))
        {
            if (ImGui::MenuItem("暗黑（Dark）")) { ImGui::StyleColorsDark(); }
            if (ImGui::MenuItem("明亮（Light）")) { ImGui::StyleColorsLight(); }
            if (ImGui::MenuItem("经典（Classic）")) { ImGui::StyleColorsClassic(); }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}
