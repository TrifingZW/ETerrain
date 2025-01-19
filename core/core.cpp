//
// Created by TrifingZW on 24-11-17.
//

#include "core.h"

GraphicsDevice* Core::_graphicsDevice = nullptr;
SpriteBatch* Core::_spriteBatch = nullptr;

Core::~Core()
{
    delete _spriteBatch;
    delete _graphicsDevice;
}

Core& Core::Instance()
{
    static Core core;
    return core;
}

void Core::Renderings()
{
    RootNode.TraverseChildren([](Node* node) { node->Rendering(*_spriteBatch); });
}

void Core::ImGuiAll()
{
    RootNode.TraverseChildren([](Node* node) { node->Gui(); });
}

void Core::InitTree()
{
    _graphicsDevice = new GraphicsDevice();
    _spriteBatch = new SpriteBatch(_graphicsDevice);

    RootNode.TraverseChildren(
        [](Node* node)
        {
            node->Init();
            node->Ready();
        }
    );
}

void Core::ProcessTree(const float delta)
{
    RootNode.TraverseChildren([delta](Node* node) { node->Process(delta); });
}

void Core::Input(const int key)
{
    RootNode.TraverseChildren([key](Node* node) { node->Process(static_cast<float>(key)); });
}

GraphicsDevice* Core::GetGraphicsDevice() { return _graphicsDevice; }

SpriteBatch& Core::GetSpriteBatch() { return *_spriteBatch; }
