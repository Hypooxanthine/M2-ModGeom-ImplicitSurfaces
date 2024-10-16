#include "SceneGraph/NodeEditor/CubeEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/Cube.h"

CubeEditor::CubeEditor(SceneNode* node, Cube* imp)
    : PrimitiveEditor(node, imp), m_Implicit(imp)
{}

void CubeEditor::onImgui_PrimitiveImpl()
{
    glm::vec3 corner = m_Implicit->getFirstCorner();
    ImGui::TextWrapped("First corner");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##First corner", &corner.x, .1f))
        m_Implicit->setFirstCorner(corner);
        
    corner = m_Implicit->getSecondCorner();
    ImGui::TextWrapped("Second corner");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Second corner", &corner.x, .1f))
        m_Implicit->setSecondCorner(corner);
}