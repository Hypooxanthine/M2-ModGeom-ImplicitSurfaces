#include "Implicits/Operators/TransformOperator.h"

#include <Vroom/Core/Log.h>

TransformOperator::TransformOperator(const glm::mat4& transform, AnalyticScalarField* field)
    : UnaryOperator(field), m_TransformInv(glm::inverse(transform))
{}

float TransformOperator::Value(const glm::vec3& p) const
{
    const glm::vec3& transformedP = glm::vec3(m_TransformInv * glm::vec4(p, 1.f));
    return fieldValue<0>(transformedP);
}