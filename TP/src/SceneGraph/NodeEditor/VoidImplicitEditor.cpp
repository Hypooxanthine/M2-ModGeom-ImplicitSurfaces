#include "SceneGraph/NodeEditor/VoidImplicitEditor.h"

#include "imgui.h"

#include "Implicits/Primitives/VoidImplicit.h"

VoidImplicitEditor::VoidImplicitEditor(SceneNode* node, VoidImplicit* imp)
    : NodeEditor(node), m_Implicit(imp)
{}

void VoidImplicitEditor::onImgui_Impl()
{
    
}