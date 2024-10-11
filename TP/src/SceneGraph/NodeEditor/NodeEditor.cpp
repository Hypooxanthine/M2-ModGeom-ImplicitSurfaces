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
        ImGui::TextWrapped("Node type");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##Node type", NodeType::GetUINameOfNodeType(m_Node->getNodeType()).data()))
        {
            for (int i = 0; i < static_cast<int>(NodeType::Type::COUNT); i++)
            {
                if (ImGui::Selectable(NodeType::GetUINameOfNodeType(static_cast<NodeType::Type>(i)).data()))
                    ;
            };

            ImGui::EndCombo();
        }

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