#pragma once

#include "Implicits/implicits.h"

class VoidImplicit : public AnalyticScalarField
{
public:
    VoidImplicit() = default;

    inline float Value(const glm::vec3& p) const override { return 1.f; }

    inline static consteval int GetRequiredChildrenCount() { return 0; }
};