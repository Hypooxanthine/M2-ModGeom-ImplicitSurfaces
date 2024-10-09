#pragma once

#include "Implicits/implicits.h"

class Sphere : public AnalyticScalarField
{
public:
    Sphere(const glm::vec3& center, float radius);

private:
    glm::vec3 m_Center;
    float m_Radius;
};