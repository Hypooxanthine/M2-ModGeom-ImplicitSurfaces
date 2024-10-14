#include "Implicits/Operators/BlendOperator.h"

#include "SceneGraph/NodeEditor/BlendOperatorEditor.h"

BlendOperator::BlendOperator(const AnalyticScalarField* field1, const AnalyticScalarField* field2, float radius)
    : Operator<2, 2>(field1, field2), m_Radius(radius)
{}

float BlendOperator::Value(const glm::vec3& p) const
{
    const float v0 = fieldValue(0, p);
    const float v1 = fieldValue(1, p);

    const float h = std::max(m_Radius - std::abs(v1 - v0), 0.f) / m_Radius;
    const float g = (1.f / 6.f) * m_Radius * h * h * h;

    return std::min(v0, v1) - g;
}

std::unique_ptr<NodeEditor> BlendOperator::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new BlendOperatorEditor(node, this));
}