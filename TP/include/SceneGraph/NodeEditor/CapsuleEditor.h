#pragma once

#include "SceneGraph/NodeEditor/PrimitiveEditor.h"

class Capsule;

class CapsuleEditor : public PrimitiveEditor
{
public:
    CapsuleEditor() = default;
    CapsuleEditor(SceneNode* node, Capsule* imp);

private:
    void onImgui_PrimitiveImpl() override;

private:
    Capsule* m_Implicit = nullptr;
};