#pragma once

#include "Implicits/Operators/Operator.h"

class BlendOperator : public Operator<2, 2>
{
public:
    BlendOperator(const AnalyticScalarField* field1, const AnalyticScalarField* field2, float radius = 1.f);

    float Value(const glm::vec3& p) const override;

    inline float& getRadiusRef() { return m_Radius; }

    std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);

    inline static consteval NodeType::Type GetNodeType() { return NodeType::Type::Blend; }

private:
    float m_Radius;
};