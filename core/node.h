//
// Created by TrifingZW on 24-11-18.
//
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graphics/sprite_batch.h"

class Node
{
public:
    bool IsInit = false;
    bool IsReady = false;
    std::string Name = "Node";;

    // TODO 节点系统的内存管理不合理
    virtual ~Node() = default;

    // TODO 字节的内存管理不合理
    void AddChild(std::unique_ptr<Node> object);
    [[nodiscard]] const std::vector<std::unique_ptr<Node> >& GetChildren() const;
    [[nodiscard]] Node* GetParent() const;

    void InitTree();
    void ReadyTree();
    void ProcessTree(float delta);
    void RenderingTree(SpriteBatch* spriteBatch);
    void GuiTree();
    void InputTree(int key);
    virtual void Init();
    virtual void Ready();
    virtual void Process(float delta);
    virtual void Rendering(SpriteBatch& spriteBatch);
    virtual void Gui();
    virtual void Input(int key);

private:
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node> > children;
};
