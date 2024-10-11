#include "SceneGraph/NodeType.h"

#include "SceneGraph/SceneNode.h"

#include "Implicits/Operators/UnionOperator.h"
#include "Implicits/Operators/IntersectionOperator.h"
#include "Implicits/Operators/BlendOperator.h"
#include "Implicits/Operators/DifferenceOperator.h"
#include "Implicits/Operators/TransformOperator.h"
#include "Implicits/Primitives/VoidImplicit.h"
#include "Implicits/Primitives/Sphere.h"

std::string_view NodeType::GetUINameOfNodeType(const Type& nodeType)
{
    switch (nodeType)
    {
    case Type::Union:
        return "Union";
    case Type::Intersection:
        return "Intersection";
    case Type::Blend:
        return "Blend";
    case Type::Difference:
        return "Difference";
    case Type::Transform:
        return "Transform";
    case Type::Empty:
        return "Empty";
    case Type::Sphere:
        return "Sphere";
    default:
        return "Undefined node type";
    }
}

SceneNode NodeType::CreateNodeOfType(const Type& nodeType, SceneGraph* graph)
{
    const std::string nodeName = std::string(GetUINameOfNodeType(nodeType));
    switch (nodeType)
    {
    case Type::Union:
        return SceneNode::CreateNode<UnionOperator>(graph, nodeName);
    case Type::Intersection:
        return SceneNode::CreateNode<IntersectionOperator>(graph, nodeName);
    case Type::Blend:
        return SceneNode::CreateNode<BlendOperator>(graph, nodeName);
    case Type::Difference:
        return SceneNode::CreateNode<DifferenceOperator>(graph, nodeName);
    case Type::Transform:
        return SceneNode::CreateNode<TransformOperator>(graph, nodeName);
    case Type::Sphere:
        return SceneNode::CreateLeaf<Sphere>(graph, nodeName);
    case Type::Empty:
    default:
        return SceneNode::CreateLeaf<VoidImplicit>(graph, nodeName);
    }
}