#pragma once

#include "Implicits/Operators/Operator.h"

class IntersectionOperator : public ArbitraryChildrenCountOperator<1>
{
public:

    template <typename... Fields>
    IntersectionOperator(const Fields*... fields)
        : ArbitraryChildrenCountOperator<1>(fields...)
    {}

    float Value(const glm::vec3& p) const override;

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Intersection; }
};