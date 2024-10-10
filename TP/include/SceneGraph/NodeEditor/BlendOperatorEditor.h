#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class BlendOperatorEditor : public NodeEditor
{
public:
    BlendOperatorEditor() = default;
    BlendOperatorEditor(SceneNode* node);

private:
    void onImgui_Impl() override;

private:

};