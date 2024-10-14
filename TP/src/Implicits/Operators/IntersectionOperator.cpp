#include "Implicits/Operators/IntersectionOperator.h"

float IntersectionOperator::Value(const glm::vec3& p) const
{
    float maxVal = fieldValue(0, p);

    for (size_t i = 1; i < getFieldsCount(); i++)
    {
        maxVal = std::max(maxVal, fieldValue(i, p));
    }

    return maxVal;
}