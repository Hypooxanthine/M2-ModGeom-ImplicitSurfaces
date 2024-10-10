#include "SceneGraph/SceneGraph.h"

#include "imgui.h"

#include "Implicits/Operators/BlendOperator.h"
#include "Implicits/Primitives/Sphere.h"

SceneGraph::SceneGraph()
    : m_Root(SceneNode::CreateNode<BlendOperator>(this, "Blending", 1.f))
{
    m_Root.setChildNode(0, SceneNode::CreateLeaf<Sphere>(this, "Sphere 1", glm::vec3(0.f), 1.5f));
    m_Root.setChildNode(1, SceneNode::CreateLeaf<Sphere>(this, "Sphere 2", glm::vec3(1.f, 0.f, 0.f), 1.f));
}

void SceneGraph::notifySelection(SceneNode* node)
{
    if (m_SelectedNode == node) // Clicking again on a node will unselect it
    {
        node->setSelected(false);
        m_SelectedNode = nullptr;
    }
    else if (m_SelectedNode == nullptr) // When nothing is selected, clicking on a node will select it
    {
        node->setSelected(true);
        m_SelectedNode = node;
    }
    else // When another node was selected, we change the selected one
    {
        m_SelectedNode->setSelected(false);
        node->setSelected(true);
        m_SelectedNode = node;
    }
}

void SceneGraph::onImgui()
{
    if (ImGui::Begin("Scene graph"))
    {
        m_Root.onImgui();

        ImGui::End();
    }
}