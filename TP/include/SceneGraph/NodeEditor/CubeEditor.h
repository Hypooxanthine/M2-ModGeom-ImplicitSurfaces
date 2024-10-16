#pragma once

#include "SceneGraph/NodeEditor/PrimitiveEditor.h"

class Cube;

class CubeEditor : public PrimitiveEditor
{
public:
    CubeEditor() = default;
    CubeEditor(SceneNode* node, Cube* imp);

private:
    void onImgui_PrimitiveImpl() override;

private:
    Cube* m_Implicit = nullptr;
};