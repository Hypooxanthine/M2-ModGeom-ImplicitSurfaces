#pragma once

#include "Implicits/Operators/Operator.h"

class BlendOperator : public BinaryOperator
{
public:
    BlendOperator(float radius, const AnalyticScalarField* field1, const AnalyticScalarField* field2);

    float Value(const glm::vec3& p) const override;

    inline float& getRadiusRef() { return m_Radius; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Blend; }

private:
    float m_Radius;
};