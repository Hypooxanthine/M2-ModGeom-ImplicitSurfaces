#pragma once

#include "SceneGraph/NodeEditor/PrimitiveEditor.h"

class Torus;

class TorusEditor : public PrimitiveEditor
{
public:
    TorusEditor() = default;
    TorusEditor(SceneNode* node, Torus* imp);

private:
    void onImgui_PrimitiveImpl() override;

private:
    Torus* m_Implicit = nullptr;
};