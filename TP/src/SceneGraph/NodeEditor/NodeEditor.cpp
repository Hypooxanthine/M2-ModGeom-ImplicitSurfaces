#include "SceneGraph/NodeEditor/NodeEditor.h"

#include "imgui.h"

#include "SceneGraph/SceneNode.h"

NodeEditor::NodeEditor(SceneNode* node)
    : m_Node(node)
{}

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