//
// Created by TrifingZW on 24-11-17.
//
#pragma once

#include "editor.h"
#include "graphics/sprite_batch.h"

class Core {
public:
    static  GraphicsDevice GraphicsDevice;
    static SpriteBatch SpriteBatch;
    Editor RootNode{};

    // 静态方法，获取单例实例
    static Core &instance() {
        static Core core;
        return core;
    }

    ~Core() = default;

    void Renderings();
    void ImGuiAll();

    void InitTree();
    void ProcessTree(float delta);
    void Input(int key);
};
