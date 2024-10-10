#pragma once

#include "SceneGraph/NodeEditor/NodeEditor.h"

class Sphere;

class SphereEditor : public NodeEditor
{
public:
    SphereEditor() = default;
    SphereEditor(SceneNode* node, Sphere* imp);

private:
    void onImgui_Impl() override;

private:
    Sphere* m_Implicit = nullptr;
};