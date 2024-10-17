#include "SceneGraph/SceneGraph.h"

#include "imgui.h"

#include "SceneGraph/NodeType.h"

#include "SceneGraph/NodeEditor/BlendOperatorEditor.h"

SceneGraph::SceneGraph()
    : m_Root(NodeType::CreateNodeOfType(NodeType::Type::Union, this))
{
    m_NodeEditor = std::make_unique<NodeEditor>();
}

void SceneGraph::notifySelection(SceneNode* node)
{
    if (m_NodeEditor->getNode() == node) // Clicking again on a node will unselect it
    {
        node->setSelected(false);
        m_NodeEditor->disable();
    }
    // When nothing is selected, clicking on a node will select it
    // Or when another node was selected, we change the selected one
    else
    {
        if (m_NodeEditor->getNode() != nullptr)
            m_NodeEditor->getNode()->setSelected(false);
        m_NodeEditor = std::move(node->getImplicit()->instanciateEditor(node));
        node->setSelected(true);
    }
}

void SceneGraph::closeNodeEditor()
{
    m_NodeEditor->disable();
}

void SceneGraph::onImgui()
{
    m_NodeEditor->onImgui();

    if (ImGui::Begin("Scene graph"))
    {
        m_Root->onImgui();

        ImGui::End();
    }

    if (m_NodeEditor->askedRefresh())
    {
        SceneNode* node = m_NodeEditor->getNode();
        m_NodeEditor = std::move(node->getImplicit()->instanciateEditor(node));
        m_NodeEditor->getNode()->setSelected(true);
    }
}