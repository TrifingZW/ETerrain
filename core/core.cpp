//
// Created by TrifingZW on 24-11-17.
//

#include "core.h"

#include <random>

GraphicsDevice* Core::_graphicsDevice = nullptr;
SpriteBatch* Core::_spriteBatch = nullptr;

Core::~Core()
{
    delete _spriteBatch;
    delete _graphicsDevice;
}

Core& Core::Instance() {
    static Core core;
    return core;
}

void Core::Renderings()
{
    RootNode.RenderingTree(_spriteBatch);
}

void Core::ImGuiAll()
{
    RootNode.GuiTree();
}

void Core::InitTree()
{
    _graphicsDevice = new GraphicsDevice();
    _spriteBatch = new SpriteBatch(_graphicsDevice);

    RootNode.InitTree();
    RootNode.ReadyTree();
}

void Core::ProcessTree(const float delta)
{
    RootNode.ProcessTree(delta);
}

void Core::Input(const int key)
{
    RootNode.InputTree(key);
}

GraphicsDevice* Core::GetGraphicsDevice() { return _graphicsDevice; }

SpriteBatch& Core::GetSpriteBatch() { return *_spriteBatch; }
