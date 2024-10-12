#pragma once

#include <memory>

#include "SceneGraph/SceneNode.h"
#include "SceneGraph/NodeEditor/NodeEditor.h"

class SceneGraph
{
public:
    SceneGraph();
    ~SceneGraph() = default;

    // Getters

    inline const SceneNode* getRoot() const { return m_Root.get(); }

    // For nodes

    void notifySelection(SceneNode* node);

    // GUI

    void onImgui();

private:
    std::unique_ptr<SceneNode> m_Root;
    std::unique_ptr<NodeEditor> m_NodeEditor = nullptr;
};