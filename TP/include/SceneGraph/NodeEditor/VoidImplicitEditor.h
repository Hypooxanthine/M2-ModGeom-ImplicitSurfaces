#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class VoidImplicit;

class VoidImplicitEditor : public NodeEditor
{
public:
    VoidImplicitEditor() = default;
    VoidImplicitEditor(SceneNode* node, VoidImplicit* imp);

private:
    void onImgui_Impl() override;

private:
    VoidImplicit* m_Implicit = nullptr;
};