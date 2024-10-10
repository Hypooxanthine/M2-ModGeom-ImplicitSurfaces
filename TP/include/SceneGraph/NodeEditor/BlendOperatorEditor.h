#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class BlendOperator;

class BlendOperatorEditor : public NodeEditor
{
public:
    BlendOperatorEditor() = default;
    BlendOperatorEditor(SceneNode* node, BlendOperator* imp);

private:
    void onImgui_Impl() override;

private:
    BlendOperator* m_Implicit = nullptr;
};