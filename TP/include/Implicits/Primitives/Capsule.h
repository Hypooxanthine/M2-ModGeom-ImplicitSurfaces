#pragma once

#include "Implicits/Primitives/Primitive.h"

class Capsule : public Primitive
{
public:
    Capsule(const glm::vec3& A = glm::vec3(0.f, -.5f, 0.f), const glm::vec3& B = glm::vec3(0.f, .5f, 0.f), float radius = .5f);
    
    inline const glm::vec3& getA() const { return m_A; }

    inline const glm::vec3& getB() const { return m_B; }

    inline float getRadius() const { return m_Radius; }

    void setA(const glm::vec3& firstCorner);

    void setB(const glm::vec3& secondCorner);

    inline void setRadius(float radius) { m_Radius = radius; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Capsule; }

protected:
    float value_PrimitiveImpl(const glm::vec3&) const override;

private:
    glm::vec3 m_A, m_B;
    float m_Radius;

    glm::vec3 m_AB;
};