//
// Created by TrifingZW on 24-11-17.
//
#pragma once

#include <imgui.h>

#include "scene/node.h"
#include "editor/load_resources.h"
#include "editor/panel/game_panel_2d.h"
#include "editor/panel/game_panel_3d.h"
#include "editor/panel/hex_panel.h"

class Editor : public Node
{
public:
    HexPanel* hexPanel = nullptr;
    GamePanel2D* gamePanel2d = nullptr;
    GamePanel3D* gamePanel3d = nullptr;
    static LoadResources* loadResources;

    explicit Editor();
    ~Editor() override;

    void Init() override;
    void Gui() override;
};
