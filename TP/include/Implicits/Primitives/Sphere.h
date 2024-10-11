#pragma once

#include "Implicits/implicits.h"

class Sphere : public AnalyticScalarField
{
public:
    Sphere(const glm::vec3& center = glm::vec3(0.f), float radius = 1.f);

    virtual float Value(const glm::vec3&) const override;

    inline float& getRadiusRef() { return m_Radius; }

    inline glm::vec3& getCenterRef() { return m_Center; }

    inline static consteval int GetRequiredChildrenCount() { return 0; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Sphere; }

private:
    glm::vec3 m_Center;
    float m_Radius;
};