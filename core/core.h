//
// Created by TrifingZW on 24-11-17.
//

#pragma once

#include "editor.h"
#include "graphics/graphics_device.h"
#include "graphics/sprite_batch.h"

class Core
{
    static GraphicsDevice* _graphicsDevice;
    static SpriteBatch* _spriteBatch;

public:
    Editor RootNode{};

    // 静态方法，获取单例实例
    static Core& Instance()
    {
        static Core core;
        return core;
    }

    ~Core() = default;

    void Renderings();
    void ImGuiAll();

    void InitTree();
    void ProcessTree(float delta);
    void Input(int key);

    static GraphicsDevice* GetGraphicsDevice();
    static SpriteBatch& GetSpriteBatch();
};
