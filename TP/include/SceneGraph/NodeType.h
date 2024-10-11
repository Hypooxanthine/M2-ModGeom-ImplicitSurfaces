#pragma once

#include <string_view>

class SceneNode;
class SceneGraph;

class NodeType
{
public:
    enum class Type
    {
        Union = 0, Intersection, Blend, Difference, Transform,
        Empty, Sphere,
        COUNT,
        InvalidNode = -1
    };

public:
    static std::string_view GetUINameOfNodeType(const Type& nodeType);
    static SceneNode CreateNodeOfType(const Type& nodeType, SceneGraph* graph);
};
