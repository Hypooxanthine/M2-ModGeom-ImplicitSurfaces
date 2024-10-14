#include "Implicits/Primitives/Primitive.h"

#include <Vroom/Core/Log.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Primitive::Primitive()
    : Primitive(glm::mat4(1.f))
{}

Primitive::Primitive(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation)
    : m_Translation(translation), m_Scale(scale), m_Rotation(rotation), m_TransformDirty(true)
{}

Primitive::Primitive(const glm::mat4& transform)
    : m_Transform(transform), m_TransformInv(glm::inverse(transform)), m_TransformDirty(false)
{}

float Primitive::Value(const glm::vec3& p) const
{
    if (m_TransformDirty)
        updateTransform();
        
    const glm::vec3 transformedP = glm::vec3(m_TransformInv * glm::vec4(p, 1.f));
    return value_PrimitiveImpl(transformedP);
}

void Primitive::setTransform(const glm::mat4& transform)
{
    glm::qua orientation(1.f, 0.f, 0.f, 0.f);
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, m_Scale, orientation, m_Translation, skew, perspective);
    m_Rotation = glm::eulerAngles(orientation);

    m_Transform = transform;
    m_TransformInv = glm::inverse(m_Transform);
}

void Primitive::updateTransform() const
{
    m_Transform = glm::translate(glm::mat4(1.f), m_Translation) *
        glm::rotate(glm::mat4(1.f), m_Rotation.x, glm::vec3(1.f, 0.f, 0.f)) *
        glm::rotate(glm::mat4(1.f), m_Rotation.y, glm::vec3(0.f, 1.f, 0.f)) *
        glm::rotate(glm::mat4(1.f), m_Rotation.z, glm::vec3(0.f, 0.f, 1.f)) *
        glm::scale(glm::mat4(1.f), m_Scale);

    m_TransformInv = glm::inverse(m_Transform);

    m_TransformDirty = false;
}