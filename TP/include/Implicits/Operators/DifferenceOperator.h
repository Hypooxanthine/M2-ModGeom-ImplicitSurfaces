#pragma once

#include "Implicits/Operators/Operator.h"

class DifferenceOperator : public BinaryOperator
{
public:
    DifferenceOperator(const AnalyticScalarField* field1, const AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Difference; }
};