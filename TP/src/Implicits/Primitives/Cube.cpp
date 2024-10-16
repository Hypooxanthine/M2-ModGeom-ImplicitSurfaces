#include "Implicits/Primitives/Cube.h"

#include "SceneGraph/NodeEditor/CubeEditor.h"

#include <glm/gtx/norm.hpp>

Cube::Cube(const glm::vec3& firstCorner, const glm::vec3& secondCorner)
    : Primitive(), m_FirstCorner(firstCorner), m_SecondCorner(secondCorner)
{}

template <typename... Args>
static float Max(float a, Args... args)
{
    return std::max(a, Max(args...));
}

template<>
static float Max(float a)
{
    return a;
}

float Cube::value_PrimitiveImpl(const glm::vec3& p) const
{
    return Max(
        m_FirstCorner.x - p.x,
        p.x - m_SecondCorner.x,
        m_FirstCorner.y - p.y,
        p.y - m_SecondCorner.y,
        m_FirstCorner.z - p.z,
        p.z - m_SecondCorner.z
    );
}

std::unique_ptr<NodeEditor> Cube::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new CubeEditor(node, this));
}