#pragma once

#include <array>

#include "Implicits/implicits.h"

template <int N>
class Operator : public AnalyticScalarField
{
public:
    Operator() = delete;
    
    template <typename... Fields>
    Operator(Fields&&... fields)
        : m_Fields{ std::forward<Fields>(fields)... }
    {}

    virtual float Value(const glm::vec3& p) const override = 0;

protected:
    template <int F>
    inline float fieldValue(const glm::vec3& p) const
    {
        return m_Fields[F]->Value(p);
    }

private:
    std::array<AnalyticScalarField*, N> m_Fields;
};

using UnaryOperator = Operator<1>;
using BinaryOperator = Operator<2>;