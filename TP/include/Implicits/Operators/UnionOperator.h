#pragma once

#include "Implicits/Operators/Operator.h"

class UnionOperator : public BinaryOperator
{
public:
    UnionOperator(AnalyticScalarField* field1, AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;
};