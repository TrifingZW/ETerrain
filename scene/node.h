//
// Created by TrifingZW on 24-11-18.
//
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "core/graphics/sprite_batch.h"

class Node
{
public:
    bool IsInit = false;
    bool IsReady = false;
    std::string Name = "Node";

    // TODO 节点系统的内存管理不合理
    virtual ~Node() = default;

    // TODO 字节的内存管理不合理
    void AddChild(Node* object);
    [[nodiscard]] bool HasChild() const;
    [[nodiscard]] const std::vector<Node *>& GetChildren() const;
    [[nodiscard]] Node* GetParent() const;
    void TraverseChildren(const std::function<void(Node*)>& func);

    virtual void Init();
    virtual void Ready();
    virtual void Process(float delta);
    virtual void Rendering(SpriteBatch& spriteBatch);
    virtual void Gui();
    virtual void Input(int key);

private:
    Node* parent = nullptr;
    std::vector<Node *> children{};
};
