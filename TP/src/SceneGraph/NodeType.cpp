#include "SceneGraph/NodeType.h"

#include "SceneGraph/SceneNode.h"

#include "Implicits/Operators/UnionOperator.h"
#include "Implicits/Operators/IntersectionOperator.h"
#include "Implicits/Operators/BlendOperator.h"
#include "Implicits/Operators/DifferenceOperator.h"
#include "Implicits/Operators/TransformOperator.h"
#include "Implicits/Primitives/VoidImplicit.h"
#include "Implicits/Primitives/Sphere.h"
#include "Implicits/Primitives/Cube.h"
#include "Implicits/Primitives/Capsule.h"
#include "Implicits/Primitives/Torus.h"

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
    case Type::Cube:
        return "Cube";
    case Type::Capsule:
        return "Capsule";
    case Type::Torus:
        return "Torus";
    default:
        return "Undefined node type";
    }
}

std::unique_ptr<SceneNode> NodeType::CreateNodeOfType(const Type& nodeType, SceneGraph* graph)
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
    case Type::Cube:
        return SceneNode::CreateLeaf<Cube>(graph, nodeName);
    case Type::Capsule:
        return SceneNode::CreateLeaf<Capsule>(graph, nodeName);
    case Type::Torus:
        return SceneNode::CreateLeaf<Torus>(graph, nodeName);
    case Type::Empty:
    default:
        return SceneNode::CreateLeaf<VoidImplicit>(graph, nodeName);
    }
}