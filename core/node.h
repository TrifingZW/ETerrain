//
// Created by TrifingZW on 24-11-18.
//
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "graphics/sprite_batch.h"

class Node {
public:
    bool IsInit = false;
    bool IsReady = false;
    std::string Name = "Node";;

    virtual ~Node() = default;

    void AddChild(std::unique_ptr<Node> object) {
        object->parent = this;
        children.push_back(std::move(object));
        if (IsInit)
            object->InitTree();
        if (IsReady)
            object->ReadyTree();
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Node> > &GetChildren() const {
        return children;
    }

    [[nodiscard]] Node *GetParent() const {
        return parent;
    }

    void InitTree();
    void ReadyTree();
    void ProcessTree(float delta);
    void RenderingTree(const SpriteBatch &spriteBatch);
    void GuiTree();
    void InputTree(int key);
    virtual void Init();
    virtual void Ready();
    virtual void Process(float delta);
    virtual void Rendering(const SpriteBatch &spriteBatch);
    virtual void Gui();
    virtual void Input(int key);

private:
    Node *parent = nullptr;
    std::vector<std::unique_ptr<Node> > children;
};
