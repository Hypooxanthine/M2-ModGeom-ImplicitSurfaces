#pragma once

#include <string_view>

class NodeType
{
public:
    enum class Type
    {
        Union = 0, Intersection, Blend, Difference, Transform,
        Empty, Sphere,
        COUNT,
        InvalidNode
    };

public:
    static std::string_view GetUINameOfNodeType(const Type& nodeType);
};
