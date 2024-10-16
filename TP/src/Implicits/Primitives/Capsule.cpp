#include "Implicits/Primitives/Capsule.h"

#include "SceneGraph/NodeEditor/CapsuleEditor.h"

#include <glm/gtx/norm.hpp>

Capsule::Capsule(const glm::vec3& firstCorner, const glm::vec3& secondCorner, float radius)
    : Primitive(), m_A(firstCorner), m_B(secondCorner), m_Radius(radius)
{
    m_AB = glm::normalize(m_B - m_A);
}

void Capsule::setA(const glm::vec3& firstCorner)
{
    m_A = firstCorner;
    m_AB = glm::normalize(m_B - m_A);
}

void Capsule::setB(const glm::vec3& secondCorner)
{
    m_B = secondCorner;
    m_AB = glm::normalize(m_B - m_A);
}

float Capsule::value_PrimitiveImpl(const glm::vec3& p) const
{
    const glm::vec3 projected = m_A + m_AB * glm::dot(p - m_A, m_AB);

    if (glm::dot(projected - m_A, m_AB) < 0.f)
        return glm::length2(p - m_A) - m_Radius * m_Radius;
    else if (glm::dot(projected - m_B, -m_AB) < 0.f)
        return glm::length2(p - m_B) - m_Radius * m_Radius;
    else
        return glm::length2(p - projected) - m_Radius * m_Radius;
}

std::unique_ptr<NodeEditor> Capsule::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new CapsuleEditor(node, this));
}