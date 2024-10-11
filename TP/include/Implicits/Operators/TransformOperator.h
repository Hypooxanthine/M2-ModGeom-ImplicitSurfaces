#pragma once

#include "Implicits/Operators/Operator.h"

class TransformOperator : public UnaryOperator
{
public:
    TransformOperator(const AnalyticScalarField* field, const glm::mat4& transform = glm::mat4(1.f));

    float Value(const glm::vec3& p) const override;

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Transform; }

private:
    glm::mat4 m_TransformInv;
};