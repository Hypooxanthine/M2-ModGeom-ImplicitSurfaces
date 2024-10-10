#pragma once

#include "Implicits/implicits.h"

class Sphere : public AnalyticScalarField
{
public:
    Sphere(const glm::vec3& center, float radius);

    virtual float Value(const glm::vec3&) const override;

    inline float& getRadiusRef() { return m_Radius; }

    inline glm::vec3& getCenterRef() { return m_Center; }

    inline static consteval int GetRequiredChildrenCount() { return 0; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

private:
    glm::vec3 m_Center;
    float m_Radius;
};