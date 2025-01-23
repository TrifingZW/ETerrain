//
// Created by TrifingZW on 24-11-17.
//

#include "core.h"

GraphicsDevice* Core::_graphicsDevice = nullptr;
SpriteBatch* Core::_spriteBatch = nullptr;

Core::Core()
{
    RootNode = new Editor();
    RootNode->Root = true;
}

Core::~Core()
{
    delete _spriteBatch;
    delete _graphicsDevice;
    delete RootNode;
}

Core& Core::Instance()
{
    static Core core;
    return core;
}


void Core::Renderings() const
{
    RootNode->TraverseChildren([](Node* node) { node->Rendering(*_spriteBatch); });
}

void Core::ImGuiAll() const
{
    RootNode->TraverseChildren([](Node* node) { node->Gui(); });
}

void Core::InitTree() const
{
    _graphicsDevice = new GraphicsDevice();
    _spriteBatch = new SpriteBatch(_graphicsDevice);

    RootNode->TraverseChildren(
        [](Node* node)
        {
            node->Init();
            node->Ready();
        }
    );
}

void Core::ProcessTree(const double delta) const
{
    RootNode->TraverseChildren([delta](Node* node) { node->Process(delta); });
}

void Core::Input(const int key) const
{
    RootNode->TraverseChildren([key](Node* node) { node->Input(key); });
}

GraphicsDevice* Core::GetGraphicsDevice() { return _graphicsDevice; }

SpriteBatch& Core::GetSpriteBatch() { return *_spriteBatch; }
