//
// Created by TrifingZW on 24-11-17.
//

#include "core.h"

GraphicsDevice Core::GraphicsDevice{};
SpriteBatch Core::SpriteBatch = SpriteBatch::SpriteBatch(GraphicsDevice);

void Core::Renderings() {
    RootNode.RenderingTree(SpriteBatch);
}

void Core::ImGuiAll() {
    RootNode.GuiTree();
}

void Core::InitTree() {
    RootNode.InitTree();
    RootNode.ReadyTree();
}

void Core::ProcessTree(const float delta) {
    RootNode.ProcessTree(delta);
}

void Core::Input(const int key) {
    RootNode.InputTree(key);
}
