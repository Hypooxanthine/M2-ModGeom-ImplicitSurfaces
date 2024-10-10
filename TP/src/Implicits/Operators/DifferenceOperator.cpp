#include "Implicits/Operators/DifferenceOperator.h"

DifferenceOperator::DifferenceOperator(const AnalyticScalarField* field1, const AnalyticScalarField* field2)
    : BinaryOperator(field1, field2)
{}

float DifferenceOperator::Value(const glm::vec3& p) const
{
    return std::max(fieldValue<0>(p), -fieldValue<1>(p));
}