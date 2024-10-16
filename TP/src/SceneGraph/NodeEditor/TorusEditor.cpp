#include "SceneGraph/NodeEditor/TorusEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/Torus.h"

TorusEditor::TorusEditor(SceneNode* node, Torus* imp)
    : PrimitiveEditor(node, imp), m_Implicit(imp)
{}

void TorusEditor::onImgui_PrimitiveImpl()
{
    glm::vec3 center = m_Implicit->getCenter();
    ImGui::TextWrapped("Center");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Center", &center.x, .1f))
        m_Implicit->setCenter(center);

    glm::vec3 normal = m_Implicit->getNormal();
    ImGui::TextWrapped("Normal");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Normal", &normal.x, .1f))
        m_Implicit->setNormal(normal);

    float littleRadius = m_Implicit->getLittleRadius();
    ImGui::TextWrapped("Little radius");
    ImGui::SameLine();
    if (ImGui::DragFloat("##Little radius", &littleRadius, .1f))
        m_Implicit->setLittleRadius(littleRadius);

    float bigRadius = m_Implicit->getBigRadius();
    ImGui::TextWrapped("Big radius");
    ImGui::SameLine();
    if (ImGui::DragFloat("##Big radius", &bigRadius, .1f))
        m_Implicit->setBigRadius(bigRadius);
}