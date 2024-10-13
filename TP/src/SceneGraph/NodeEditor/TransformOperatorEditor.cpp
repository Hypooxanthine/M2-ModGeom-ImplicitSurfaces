#include "SceneGraph/NodeEditor/TransformOperatorEditor.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/gtc/constants.hpp>

#include "Implicits/Operators/TransformOperator.h"
#include "SceneGraph/SceneNode.h"

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
    
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList()); 
    auto& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    glm::mat4 transform = m_Implicit->getTransform();
    const auto& view = vrm::Application::Get().getGameLayer().getScene().getCamera().getView();;
    const auto& projection = vrm::Application::Get().getGameLayer().getScene().getCamera().getProjection();

    if (ImGuizmo::Manipulate(
        &view[0][0],
        &projection[0][0],
        ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE,
        ImGuizmo::MODE::WORLD,
        &transform[0][0]
    ))
    {
        m_Implicit->setTransform(transform);
    }

}