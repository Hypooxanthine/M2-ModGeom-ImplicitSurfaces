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
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE"))
            {
                SceneNode* srcNode = *(SceneNode**)payload->Data;
                SceneNode* dstNode = this;

                if (!srcNode->isAncestorOf(dstNode) && srcNode != dstNode)
                {
                    auto* srcParent = srcNode->getParent();
                    auto fieldSrc = srcParent->getFieldContaining(srcNode);

                    bool dstIsAncestorOfSrc = dstNode->isAncestorOf(srcNode);
                    bool leastThanMinChildren = srcParent->getChildrenCount() - 1 < srcParent->getMinChildrenCount();

                    setNode(std::move(srcNode->getParent()->m_Children.at(fieldSrc)), false);

                    if (!dstIsAncestorOfSrc)
                    {
                        if (leastThanMinChildren)
                            srcParent->setChildNode(fieldSrc, CreateLeaf<VoidImplicit>(m_SceneGraph, "Empty"));
                        else
                            srcParent->removeChildNode(fieldSrc);
                    }

                    m_SceneGraph->closeNodeEditor();
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked())
        {
            m_SceneGraph->notifySelection(this);
        }

        ImGui::SameLine();
        if (ImGui::SmallButton("="))
        {
        }
        
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers))
        {
            SceneNode* node = this;
            ImGui::SetDragDropPayload("NODE", &node, sizeof(node));
            ImGui::Text("%s", getNodeName().data());
            ImGui::EndDragDropSource();
        }

        for (auto& childNode : m_Children)
            childNode->onImgui();

        ImGui::TreePop();
    }

    ImGui::PopID();
}

SceneNode* SceneNode::setChildNode(size_t field, std::unique_ptr<SceneNode>&& node)
{
    if (field >= m_Children.size())
    {
        VRM_LOG_WARN("Field {} >= children count {}", field, m_Children.size());
        m_Children.reserve(field + 1);

        for (int i = m_Children.size(); i < field + 1; i++)
        {
            m_Children.emplace_back(CreateLeaf<VoidImplicit>(m_SceneGraph, "Empty"));
            m_Children.back()->setParent(this);
            m_Implicit->addField(m_Children.back()->getImplicit());
        }
    }

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

void SceneNode::removeChildNode(size_t field)
{
    VRM_ASSERT_MSG(m_Children.size() > m_MinChildrenCount, "Couldn't remove child node because children count is already at its minimum.");
    m_Implicit->removeField(field);
    m_Children.erase(std::next(m_Children.begin(), field));
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

bool SceneNode::isAncestorOf(const SceneNode* node) const
{
    if (node == nullptr)
        return false;

    if (node->getParent() == this)
        return true;

    return isAncestorOf(node->getParent());
}