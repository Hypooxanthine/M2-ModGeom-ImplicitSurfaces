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
    ImGui::SliderFloat3("##Spherecenter", reinterpret_cast<float*>(&m_Implicit->getCenterRef()), -10.f, 10.f, "%.2f");

    ImGui::TextWrapped("Sphere radius");
    ImGui::SameLine();
    ImGui::SliderFloat("##Sphereradius", &m_Implicit->getRadiusRef(), 0.f, 10.f, "%.2f");
}