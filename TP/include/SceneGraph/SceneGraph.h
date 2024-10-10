#pragma once

#include "SceneGraph/SceneNode.h"

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
    SceneNode* m_SelectedNode = nullptr;
};