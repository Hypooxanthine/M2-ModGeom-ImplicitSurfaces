#pragma once

#include "Implicits/Operators/Operator.h"

class BlendOperator : public BinaryOperator
{
public:
    BlendOperator(float radius, AnalyticScalarField* field1, AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;

private:
    float m_Radius;
};