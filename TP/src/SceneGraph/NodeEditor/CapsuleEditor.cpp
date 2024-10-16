#include "SceneGraph/NodeEditor/CapsuleEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/Capsule.h"

CapsuleEditor::CapsuleEditor(SceneNode* node, Capsule* imp)
    : PrimitiveEditor(node, imp), m_Implicit(imp)
{}

void CapsuleEditor::onImgui_PrimitiveImpl()
{
    glm::vec3 point = m_Implicit->getA();
    ImGui::TextWrapped("First point");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##First point", &point.x, .1f))
        m_Implicit->setA(point);
        
    point = m_Implicit->getB();
    ImGui::TextWrapped("Second point");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Second point", &point.x, .1f))
        m_Implicit->setB(point);

    float radius = m_Implicit->getRadius();
    ImGui::TextWrapped("Radius");
    ImGui::SameLine();
    if (ImGui::DragFloat("##Radius", &radius, .1f))
        m_Implicit->setRadius(radius);
}