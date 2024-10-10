#include "SceneGraph/NodeEditor/NodeEditor.h"

#include "imgui.h"

#include "SceneGraph/SceneNode.h"

NodeEditor::NodeEditor(SceneNode* node)
    : m_Node(node)
{}

void NodeEditor::setToggleNode(SceneNode* node)
{
    if (m_Node == node) // Clicking again on a node will unselect it
    {
        node->setSelected(false);
        m_Node = nullptr;
    }
    else if (m_Node == nullptr) // When nothing is selected, clicking on a node will select it
    {
        node->setSelected(true);
        m_Node = node;
    }
    else // When another node was selected, we change the selected one
    {
        m_Node->setSelected(false);
        node->setSelected(true);
        m_Node = node;
    }
}

void NodeEditor::onImgui()
{
    if (m_Node == nullptr) return;

    if (ImGui::Begin("Node editor"))
    {
        ImGui::TextWrapped("Name");
        ImGui::SameLine();
        ImGui::InputText(
            "##NodeName",
            m_Node->getNodeName().data(),
            m_Node->getNodeName().capacity()
        );

        onImgui_Impl();

        ImGui::End();
    }
}