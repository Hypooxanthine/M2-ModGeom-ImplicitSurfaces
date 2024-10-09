#pragma once

#include "Implicits/Operators/Operator.h"

class DifferenceOperator : public BinaryOperator
{
public:
    DifferenceOperator(AnalyticScalarField* field1, AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;
};