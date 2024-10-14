#pragma once

#include "Implicits/Primitives/Primitive.h"

class Sphere : public Primitive
{
public:
    Sphere(const glm::vec3& center = glm::vec3(0.f), float radius = 1.f);

    inline float getRadius() const { return m_Radius; }

    inline const glm::vec3& getCenter() const { return m_Center; }

    inline void setRadius(float radius) { m_Radius = radius; }

    inline void setCenter(const glm::vec3& center) { m_Center = center; }

    inline static consteval int GetRequiredChildrenCount() { return 0; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Sphere; }

protected:
    float value_PrimitiveImpl(const glm::vec3&) const override;

private:
    glm::vec3 m_Center;
    float m_Radius;
};