#include "SceneGraph/NodeEditor/TransformOperatorEditor.h"

#include "imgui.h"
#include <glm/gtc/constants.hpp>

#include "Implicits/Operators/TransformOperator.h"

TransformOperatorEditor::TransformOperatorEditor(SceneNode* node, TransformOperator* imp)
    : NodeEditor(node), m_Implicit(imp)
{}

void TransformOperatorEditor::onImgui_Impl()
{
    glm::vec3 translation = m_Implicit->getTranslation();
    if (ImGui::DragFloat3("Translation", &translation[0], 0.1f))
        m_Implicit->setTranslation(translation);
    
    glm::vec3 scale = m_Implicit->getScale();
    if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
        m_Implicit->setScale(scale);

    glm::vec3 rotation = m_Implicit->getRotation();
    if (ImGui::DragFloat3("Rotation", &rotation[0], 0.01f, 0.f, glm::two_pi<float>()))
        m_Implicit->setRotation(rotation);
}