#pragma once

#include "Implicits/Operators/Operator.h"

class IntersectionOperator : public BinaryOperator
{
public:
    IntersectionOperator(const AnalyticScalarField* field1, const AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;
};