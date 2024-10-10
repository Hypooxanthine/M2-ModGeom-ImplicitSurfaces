#include "SceneGraph/SceneGraph.h"

#include "Implicits/Primitives/VoidImplicit.h"

SceneGraph::SceneGraph()
    : m_Root(SceneNode::CreateLeaf<VoidImplicit>("Root"))
{
    
}