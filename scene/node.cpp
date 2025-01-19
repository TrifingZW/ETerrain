//
// Created by TrifingZW on 24-11-18.
//

#include "node.h"

#include <functional>

void Node::AddChild(Node* object)
{
    object->parent = this;
    if (IsInit)
    {
        object->Init();
        object->TraverseChildren([](Node* node) { node->Init(); });
    }
    if (IsReady)
    {
        object->Ready();
        object->TraverseChildren([](Node* node) { node->Ready(); });
    }
    children.push_back(object);
}

bool Node::HasChild() const { return children.empty(); }

const std::vector<Node *>& Node::GetChildren() const
{
    return children;
}

Node* Node::GetParent() const
{
    return parent;
}

void Node::TraverseChildren(const std::function<void(Node*)>& func) // NOLINT(*-no-recursion)
{
    func(this);
    for (const auto child: children)
    {
        func(child);
        if (child->HasChild())
            child->TraverseChildren(func);
    }
}

void Node::Init() {}

void Node::Ready() {}

void Node::Process(float delta) {}

void Node::Rendering(SpriteBatch& spriteBatch) {}

void Node::Gui() {}

void Node::Input(int key) {}
