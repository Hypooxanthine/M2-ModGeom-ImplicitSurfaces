#pragma once

#include "Implicits/Primitives/Primitive.h"

class Cube : public Primitive
{
public:
    Cube(const glm::vec3& firstCorner = glm::vec3(-.5f), const glm::vec3& secondCorner = glm::vec3(.5f));
    
    inline const glm::vec3& getFirstCorner() const { return m_FirstCorner; }

    inline const glm::vec3& getSecondCorner() const { return m_SecondCorner; }

    inline void setFirstCorner(const glm::vec3& firstCorner) { m_FirstCorner = firstCorner; }

    inline void setSecondCorner(const glm::vec3& secondCorner) { m_SecondCorner = secondCorner; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Cube; }

protected:
    float value_PrimitiveImpl(const glm::vec3&) const override;

private:
    glm::vec3 m_FirstCorner, m_SecondCorner;
};