//
// Created by TrifingZW on 24-11-18.
//

#include "node.h"

void Node::AddChild(std::unique_ptr<Node> object) {
    object->parent = this;
    if (IsInit)
        object->InitTree();
    if (IsReady)
        object->ReadyTree();
    children.push_back(std::move(object));
}

const std::vector<std::unique_ptr<Node>>& Node::GetChildren() const {
    return children;
}

Node* Node::GetParent() const {
    return parent;
}

void Node::InitTree() {
    Init();
    IsInit = true;
    for (const auto &child: children) {
        child->InitTree();
    }
}

void Node::ReadyTree() {
    Ready();
    IsReady = true;
    for (const auto &child: children) {
        child->ReadyTree();
    }
}

void Node::ProcessTree(const float delta) {
    Process(delta);
    for (const auto &child: children) {
        child->ProcessTree(delta);
    }
}

void Node::RenderingTree(SpriteBatch* spriteBatch) {
    Rendering(*spriteBatch);
    for (const auto &child: children) {
        child->RenderingTree(spriteBatch);
    }
}

void Node::GuiTree() {
    Gui();
    for (const auto &child: children) {
        child->GuiTree();
    }
}

void Node::InputTree(const int key) {
    Input(key);
    for (const auto &child: children) {
        child->InputTree(key);
    }
}

void Node::Init() {
}

void Node::Ready() {
}

void Node::Process(float delta) {
}

void Node::Rendering(SpriteBatch& spriteBatch) {
}

void Node::Gui() {
}

void Node::Input(int key) {
}
