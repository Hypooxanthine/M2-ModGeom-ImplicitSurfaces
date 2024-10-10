#pragma once

#include <memory>

#include "SceneGraph/SceneNode.h"
#include "SceneGraph/NodeEditor/NodeEditor.h"

class SceneGraph
{
public:
    using iterator = SceneNode::iterator;
public:
    SceneGraph();
    ~SceneGraph() = default;

    // Getters

    inline const SceneNode& getRoot() const { return m_Root; }

    // For nodes

    void notifySelection(SceneNode* node);

    // GUI

    void onImgui();

    // Iteration

    inline iterator begin() { return m_Root.begin(); }
    inline iterator end() { return m_Root.end(); }

private:
    SceneNode m_Root;
    std::unique_ptr<NodeEditor> m_NodeEditor = nullptr;
};