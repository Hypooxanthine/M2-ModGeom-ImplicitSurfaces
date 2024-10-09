#pragma once

#include "Implicits/Operators/Operator.h"

class IntersectionOperator : public BinaryOperator
{
public:
    IntersectionOperator(AnalyticScalarField* field1, AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;
};