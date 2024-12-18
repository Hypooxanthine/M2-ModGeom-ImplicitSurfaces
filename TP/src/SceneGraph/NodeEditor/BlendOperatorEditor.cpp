#include "SceneGraph/NodeEditor/BlendOperatorEditor.h"

#include "imgui.h"

#include "Implicits/Operators/BlendOperator.h"

BlendOperatorEditor::BlendOperatorEditor(SceneNode* node, BlendOperator* imp)
    : NodeEditor(node), m_Implicit(imp)
{}

void BlendOperatorEditor::onImgui_Impl()
{
    ImGui::TextWrapped("Blending radius");
    ImGui::SameLine();
    ImGui::DragFloat("##Blendingradius", &m_Implicit->getRadiusRef(), 0.1f, 0.1f, 15.f);
}