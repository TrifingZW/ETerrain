//
// Created by TrifingZW on 24-11-17.
//

#pragma once

#include "editor/editor.h"
#include "graphics/graphics_device.h"
#include "graphics/sprite_batch.h"

class Core
{
    static GraphicsDevice* _graphicsDevice;
    static SpriteBatch* _spriteBatch;

public:
    Node* RootNode = nullptr;

    Core();
    ~Core();
    // 静态方法，获取单例实例
    static Core& Instance();


    void Renderings() const;
    void ImGuiAll() const;
    void InitTree() const;
    void ProcessTree(double delta) const;
    void Input(int key) const;

    static GraphicsDevice* GetGraphicsDevice();
    static SpriteBatch& GetSpriteBatch();
};
