#include "SceneGraph/SceneGraph.h"

#include "imgui.h"

#include "Implicits/Operators/BlendOperator.h"
#include "Implicits/Primitives/Sphere.h"

#include "SceneGraph/NodeEditor/BlendOperatorEditor.h"

SceneGraph::SceneGraph()
    : m_Root(SceneNode::CreateNode<BlendOperator>(this, "Blending", 1.f))
{
    m_Root.setChildNode(0, SceneNode::CreateLeaf<Sphere>(this, "Sphere 1", glm::vec3(0.f), 1.5f));
    m_Root.setChildNode(1, SceneNode::CreateLeaf<Sphere>(this, "Sphere 2", glm::vec3(1.f, 0.f, 0.f), 1.f));

    m_NodeEditor = std::make_unique<BlendOperatorEditor>();
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
        m_NodeEditor = std::move(node->getImplicit().instanciateEditor(node));
        node->setSelected(true);
    }
}

void SceneGraph::onImgui()
{
    if (ImGui::Begin("Scene graph"))
    {
        m_Root.onImgui();

        ImGui::End();
    }

    m_NodeEditor->onImgui();
}