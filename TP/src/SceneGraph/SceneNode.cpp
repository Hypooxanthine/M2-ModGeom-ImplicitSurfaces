#include "SceneGraph/SceneNode.h"

#include "imgui.h"

void SceneNode::onImgui()
{
    ImGui::PushID(this);

    int flags = isNode() ?
        ImGuiTreeNodeFlags_DefaultOpen
        : ImGuiTreeNodeFlags_Leaf;
    
    if (ImGui::TreeNodeEx(getNodeName().c_str(), flags))
    {
        ImGui::SameLine();
        if (ImGui::Checkbox("##checkbox", &m_Selected))
        {

        }

        for (auto& childNode : m_Children)
            childNode.onImgui();
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void SceneNode::setChildNode(size_t field, SceneNode&& node)
{
    m_Implicit->setField(field, &node.getImplicit());
    m_Children.at(field) = std::move(node);
}