#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class Primitive;

class PrimitiveEditor : public NodeEditor
{
public:
    PrimitiveEditor() = default;
    PrimitiveEditor(SceneNode* node, Primitive* imp);

protected:
    virtual void onImgui_PrimitiveImpl() = 0;

private:
    void onImgui_Impl() override;

private:
    Primitive* m_ImplicitAsPrimitive = nullptr;
};