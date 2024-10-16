#pragma once

#include "Implicits/Primitives/Primitive.h"

class Torus : public Primitive
{
public:
    Torus(const glm::vec3& center = glm::vec3(0.f), const glm::vec3& normal = glm::vec3(0.f, 1.f, 0.f), float littleRadius = .25f, float bigRadius = 1.f);

    inline const glm::vec3& getCenter() const { return m_Center; }

    inline const glm::vec3& getNormal() const { return m_Normal; }

    inline float getLittleRadius() const { return m_LittleRadius; }

    inline float getBigRadius() const { return m_BigRadius; }

    inline void setCenter(const glm::vec3& center) { m_Center = center; }

    inline void setNormal(const glm::vec3& normal) { m_Normal = normal; }

    inline void setLittleRadius(float littleRadius) { m_LittleRadius = littleRadius; }

    inline void setBigRadius(float bigRadius) { m_BigRadius = bigRadius; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Torus; }

protected:
    float value_PrimitiveImpl(const glm::vec3&) const override;

private:
    glm::vec3 m_Center;
    glm::vec3 m_Normal;
    float m_LittleRadius, m_BigRadius;
};