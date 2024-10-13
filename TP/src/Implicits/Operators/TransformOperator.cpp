#include "Implicits/Operators/TransformOperator.h"

#include <Vroom/Core/Log.h>

#include <glm/gtc/matrix_transform.hpp>

#include "SceneGraph/NodeEditor/TransformOperatorEditor.h"

TransformOperator::TransformOperator(const AnalyticScalarField* field, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation)
    : UnaryOperator(field), m_Translation(translation), m_Scale(scale), m_Rotation(rotation)
{}

float TransformOperator::Value(const glm::vec3& p) const
{
    if (m_TransformDirty)
        updateTransform();
        
    const glm::vec3& transformedP = glm::vec3(m_TransformInv * glm::vec4(p, 1.f));
    return fieldValue<0>(transformedP);
}

std::unique_ptr<NodeEditor> TransformOperator::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new TransformOperatorEditor(node, this));
}

void TransformOperator::updateTransform() const
{
    m_TransformInv = glm::inverse(
        glm::translate(glm::mat4(1.f), m_Translation) *
        glm::rotate(glm::mat4(1.f), m_Rotation.x, glm::vec3(1.f, 0.f, 0.f)) *
        glm::rotate(glm::mat4(1.f), m_Rotation.y, glm::vec3(0.f, 1.f, 0.f)) *
        glm::rotate(glm::mat4(1.f), m_Rotation.z, glm::vec3(0.f, 0.f, 1.f)) *
        glm::scale(glm::mat4(1.f), m_Scale)
    );

    m_TransformDirty = false;
}