#pragma once

#include "SceneGraph/NodeEditor/PrimitiveEditor.h"

class Sphere;

class SphereEditor : public PrimitiveEditor
{
public:
    SphereEditor() = default;
    SphereEditor(SceneNode* node, Sphere* imp);

private:
    void onImgui_PrimitiveImpl() override;

private:
    Sphere* m_Implicit = nullptr;
};