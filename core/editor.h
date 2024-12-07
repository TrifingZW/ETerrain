//
// Created by TrifingZW on 24-11-17.
//
#pragma once

#include <imgui.h>

#include "node.h"
#include "nodes/load_resources.h"
#include "scene/panel/game_panel_2d.h"
#include "scene/panel/game_panel_3d.h"
#include "scene/panel/hex_panel.h"

class Editor : public Node
{
public:
    ImGuiID dockSpaceId{};
    HexPanel* hexPanel = nullptr;
    GamePanel2D* gamePanel2d = nullptr;
    GamePanel3D* gamePanel3d = nullptr;
    static LoadResources* loadResources;

    explicit Editor()
    {
        /*auto* panelHex = new HexPanel();
        hexPanel = panelHex;
        AddChild(std::unique_ptr<Node>(panelHex));*/

        auto* panel2d = new GamePanel2D();
        gamePanel2d = panel2d;
        AddChild(std::unique_ptr<Node>(panel2d));

        auto* load_resources = new LoadResources();
        loadResources = load_resources;
        AddChild(std::unique_ptr<Node>(load_resources));
    }

    ~Editor() override = default;

    void Gui() override;
};
