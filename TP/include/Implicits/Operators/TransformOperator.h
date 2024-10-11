#pragma once

#include "Implicits/Operators/Operator.h"

class TransformOperator : public UnaryOperator
{
public:
    TransformOperator(const glm::mat4& transform, const AnalyticScalarField* field);

    float Value(const glm::vec3& p) const override;

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Transform; }

private:
    glm::mat4 m_TransformInv;
};