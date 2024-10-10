#pragma once

#include "Implicits/implicits.h"

class Sphere : public AnalyticScalarField
{
public:
    Sphere(const glm::vec3& center, float radius);

    virtual float Value(const glm::vec3&) const override;

    inline static consteval int GetRequiredChildrenCount() { return 0; }

private:
    glm::vec3 m_Center;
    float m_Radius;
};