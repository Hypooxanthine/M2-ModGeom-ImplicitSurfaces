#pragma once

#include <string_view>
#include <memory>

class SceneNode;
class SceneGraph;

class NodeType
{
public:
    enum class Type
    {
        Union = 0, Intersection, Blend, Difference, Transform,
        Empty, Sphere, Cube,
        COUNT, OPERATORS_COUNT = Empty - Union, PRIMITIVES_COUNT = COUNT - Empty,
        InvalidNode = -1
    };

public:
    static std::string_view GetUINameOfNodeType(const Type& nodeType);
    static std::unique_ptr<SceneNode> CreateNodeOfType(const Type& nodeType, SceneGraph* graph);
};
