#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class TransformOperator;

class TransformOperatorEditor : public NodeEditor
{
public:
    TransformOperatorEditor() = default;
    TransformOperatorEditor(SceneNode* node, TransformOperator* imp);

private:
    void onImgui_Impl() override;

private:
    TransformOperator* m_Implicit = nullptr;
};