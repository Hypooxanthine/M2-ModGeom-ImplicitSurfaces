#include "SceneGraph/NodeEditor/SphereEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/Sphere.h"

SphereEditor::SphereEditor(SceneNode* node, Sphere* imp)
    : PrimitiveEditor(node, imp), m_Implicit(imp)
{}

void SphereEditor::onImgui_PrimitiveImpl()
{
    glm::vec3 center = m_Implicit->getCenter();
    ImGui::TextWrapped("Sphere center");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Spherecenter", &center.x, .1f))
        m_Implicit->setCenter(center);

    float radius = m_Implicit->getRadius();
    ImGui::TextWrapped("Sphere radius");
    ImGui::SameLine();
    if (ImGui::DragFloat("##Sphereradius", &radius, .1f, 0.1f, 10.f))
        m_Implicit->setRadius(radius);
}