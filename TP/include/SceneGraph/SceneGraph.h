#pragma once

#include "SceneGraph/SceneNode.h"

class SceneGraph
{
public:
    using iterator = SceneNode::iterator;
public:
    SceneGraph();
    ~SceneGraph() = default;

    inline const SceneNode& getRoot() const { return m_Root; }

    inline iterator begin() { return m_Root.begin(); }
    inline iterator end() { return m_Root.end(); }

private:
    SceneNode m_Root;
};