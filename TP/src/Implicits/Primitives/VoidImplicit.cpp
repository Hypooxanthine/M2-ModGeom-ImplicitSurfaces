#include "Implicits/Primitives/VoidImplicit.h"

#include "SceneGraph/NodeEditor/VoidImplicitEditor.h"

std::unique_ptr<NodeEditor> VoidImplicit::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new VoidImplicitEditor(node, this));
}