#include "SceneGraph/NodeType.h"

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