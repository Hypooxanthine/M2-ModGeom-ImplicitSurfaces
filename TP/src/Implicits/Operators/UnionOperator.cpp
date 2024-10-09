#include "Implicits/Operators/UnionOperator.h"

UnionOperator::UnionOperator(AnalyticScalarField* field1, AnalyticScalarField* field2)
    : BinaryOperator(field1, field2)
{}

float UnionOperator::Value(const glm::vec3& p) const
{
    return std::min(fieldValue<0>(p), fieldValue<1>(p));
}