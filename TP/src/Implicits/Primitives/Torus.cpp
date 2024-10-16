#include "Implicits/Primitives/Torus.h"

#include "SceneGraph/NodeEditor/TorusEditor.h"

#include <glm/gtx/norm.hpp>

Torus::Torus(const glm::vec3& center, const glm::vec3& normal, float littleRadius, float bigRadius)
    : Primitive(), m_Center(center), m_Normal(normal), m_LittleRadius(littleRadius), m_BigRadius(bigRadius)
{}

float Torus::value_PrimitiveImpl(const glm::vec3& p) const
{
    const float lambda = (glm::dot(m_Normal, p) - glm::dot(m_Normal, m_Center)) / glm::dot(m_Normal, m_Normal);
    const glm::vec3 projected = p - lambda * m_Normal;
    const glm::vec3 closestPoint = m_Center + glm::normalize(projected - m_Center) * m_BigRadius;
    
    return glm::length(closestPoint - p) - m_LittleRadius;
}

std::unique_ptr<NodeEditor> Torus::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new TorusEditor(node, this));
}