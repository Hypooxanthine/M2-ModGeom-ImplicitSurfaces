#include "SceneGraph/NodeEditor/SphereEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/Sphere.h"

SphereEditor::SphereEditor(SceneNode* node, Sphere* imp)
    : NodeEditor(node), m_Implicit(imp)
{}

void SphereEditor::onImgui_Impl()
{
    ImGui::TextWrapped("Sphere center");
    ImGui::SameLine();
    ImGui::DragFloat3("##Spherecenter", &m_Implicit->getCenterRef().x, .1f);

    ImGui::TextWrapped("Sphere radius");
    ImGui::SameLine();
    ImGui::DragFloat("##Sphereradius", &m_Implicit->getRadiusRef(), .1f, 0.1f, 10.f);
}