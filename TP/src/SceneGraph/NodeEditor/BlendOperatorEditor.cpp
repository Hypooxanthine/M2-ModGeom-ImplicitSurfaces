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
    ImGui::SliderFloat("##Blendingradius", &m_Implicit->getRadiusRef(), 0.f, 10.f, "%.2f", ImGuiSliderFlags_Logarithmic);
}