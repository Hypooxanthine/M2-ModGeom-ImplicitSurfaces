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
    m_NodeEditor->setToggleNode(node);
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