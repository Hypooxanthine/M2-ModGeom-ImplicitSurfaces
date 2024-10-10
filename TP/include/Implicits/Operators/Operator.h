#pragma once

#include <array>
#include <Vroom/Core/Assert.h>

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

    inline static consteval int GetRequiredChildrenCount() { return N; }

    inline void setField(size_t field, const AnalyticScalarField* asf) override
    {
        VRM_ASSERT_MSG(field < N, "Requested to edit field {} but the operator only holds {}.", field, N);
        m_Fields.at(field) = asf;
    }

protected:
    template <int F>
    inline float fieldValue(const glm::vec3& p) const
    {
        return m_Fields[F]->Value(p);
    }

private:
    std::array<const AnalyticScalarField*, N> m_Fields;
};

using UnaryOperator = Operator<1>;
using BinaryOperator = Operator<2>;