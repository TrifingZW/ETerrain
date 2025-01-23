//
// Created by TrifingZW on 24-11-17.
//

#include "editor.h"

#include <imgui_internal.h>

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

void Editor::Init()
{
}

void Editor::Gui() {}
