#include "SceneGraph/SceneNode.h"

#include <Vroom/Core/Log.h>

#include "imgui.h"

#include "SceneGraph/SceneGraph.h"

float SceneNode::value(const glm::vec3& p) const
{
    return m_Implicit->Value(p);
}

void SceneNode::onImgui()
{
    ImGui::PushID(this);

    int flags = isNode() ?
        (ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)
        : (ImGuiTreeNodeFlags_Leaf);
    
    if (m_Selected)
        flags |= ImGuiTreeNodeFlags_Selected;
    
    if (ImGui::TreeNodeEx(getNodeName().c_str(), flags))
    {
        if (ImGui::IsItemClicked())
        {
            m_SceneGraph->notifySelection(this);
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