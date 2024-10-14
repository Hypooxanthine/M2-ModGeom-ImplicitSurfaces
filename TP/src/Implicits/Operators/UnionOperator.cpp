#include "Implicits/Operators/UnionOperator.h"

float UnionOperator::Value(const glm::vec3& p) const
{
    float minVal = fieldValue(0, p);

    for (size_t i = 1; i < getFieldsCount(); i++)
    {
        minVal = std::min(minVal, fieldValue(i, p));
    }

    return minVal;
}
