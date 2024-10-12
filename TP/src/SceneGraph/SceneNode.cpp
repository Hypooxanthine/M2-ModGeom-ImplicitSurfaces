#include "SceneGraph/SceneNode.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Core/Assert.h>

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
            childNode->onImgui();

        ImGui::TreePop();
    }

    ImGui::PopID();
}

SceneNode* SceneNode::setChildNode(size_t field, std::unique_ptr<SceneNode>&& node)
{
    m_Implicit->setField(field, node->getImplicit());
    node->setParent(this);
    m_Children.at(field) = std::move(node);
    for (auto& child : m_Children.at(field)->getChildren())
        child->setParent(m_Children.at(field).get());

    return m_Children.at(field).get();
}

SceneNode* SceneNode::setNode(std::unique_ptr<SceneNode>&& node, bool preserveChildren)
{
    VRM_ASSERT_MSG(node->getSceneGraph() == m_SceneGraph, "Scene graph mismatch.");

    if (preserveChildren)
        for (int i = 0; i < getChildrenCount() && i < node->getChildrenCount(); i++)
            node->setChildNode(i, std::move(m_Children.at(i)));
    
    if (m_Parent != nullptr)
        m_Parent->getImplicit()->setField(m_Parent->getFieldContaining(this), node->getImplicit());

    auto* parent = m_Parent;
    *this = std::move(*node.get());
    m_Parent = parent;

    for (auto& child : m_Children)
        child->setParent(this);

    return this;
}

size_t SceneNode::getFieldContaining(const SceneNode* node) const
{
    for (size_t i = 0; i < m_Children.size(); i++)
    {
        if (m_Children.at(i).get() == node)
            return i;
    }

    VRM_ASSERT_MSG(false, "Node not found in children.");
}