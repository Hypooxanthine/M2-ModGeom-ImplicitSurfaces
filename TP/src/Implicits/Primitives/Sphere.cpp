#include "Implicits/Primitives/Sphere.h"

#include "SceneGraph/NodeEditor/SphereEditor.h"

#include <glm/gtx/norm.hpp>

Sphere::Sphere(const glm::vec3& center, float radius)
    : m_Center(center), m_Radius(radius)
{}

float Sphere::Value(const glm::vec3& p) const
{
    return glm::length2(p - m_Center) - m_Radius * m_Radius;
}

std::unique_ptr<NodeEditor> Sphere::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new SphereEditor(node, this));
}