#pragma once

#include "Implicits/implicits.h"

class Primitive : public AnalyticScalarField
{
public:
    Primitive();

    Primitive(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation);

    Primitive(const glm::mat4& transform);

    inline static consteval int GetRequiredChildrenCount() { return 0; }

    float Value(const glm::vec3& p) const override;

    inline void setTranslation(const glm::vec3& translation) { m_Translation = translation; m_TransformDirty = true; }

    inline void setScale(const glm::vec3& scale) { m_Scale = scale; m_TransformDirty = true; }

    inline void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; m_TransformDirty = true; }

    void setTransform(const glm::mat4& transform);

    inline const glm::vec3& getTranslation() const { return m_Translation; }

    inline const glm::vec3& getScale() const { return m_Scale; }

    inline const glm::vec3& getRotation() const { return m_Rotation; }

    inline const glm::mat4& getTransformInv() const { return m_TransformInv; }

    inline glm::mat4 getTransform() const { return m_Transform; }

protected:
    virtual float value_PrimitiveImpl(const glm::vec3& p) const = 0;

private:
    void updateTransform() const;

private:
    mutable glm::mat4 m_Transform;
    mutable glm::mat4 m_TransformInv;
    mutable bool m_TransformDirty = true;

    glm::vec3 m_Translation;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

};