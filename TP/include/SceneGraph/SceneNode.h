#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Implicits/implicits.h"
#include "Implicits/Primitives/VoidImplicit.h"
#include "SceneGraph/NodeType.h"

class SceneGraph;

// arra_to_tuple function from cppreference : https://en.cppreference.com/w/cpp/utility/integer_sequence
template <typename Array, std::size_t... I>
inline constexpr auto array_to_tuple_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template <typename T, std::size_t N, typename Indx = std::make_index_sequence<N>>
inline constexpr auto array_to_tuple(const std::array<T, N>& a)
{
    return array_to_tuple_impl(a, Indx{});
}

class SceneNode
{
public:
    using Container = std::vector<std::unique_ptr<SceneNode>>;

public:

    SceneNode(SceneNode&& other) = default;

    SceneNode& operator=(SceneNode&& other) = default;

    ~SceneNode() = default;

    // Static
    /**
     * Only for leaves, analytic scalar field must be a primitive
     */
    template <typename T, typename... Args>
    static std::unique_ptr<SceneNode> CreateLeaf(SceneGraph* graph, const std::string& nodeName, Args&&... args)
    {
        static_assert(T::GetRequiredChildrenCount() == 0);
        SceneNode* n = new SceneNode();
        n->m_SceneGraph = graph;
        n->m_NodeName = nodeName;
        n->m_NodeType = T::GetNodeType();
        n->m_Implicit = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        n->m_IsLeaf = true;

        return std::unique_ptr<SceneNode>(n);
    }

    template <typename T, typename... Args>
    static std::unique_ptr<SceneNode> CreateNode(SceneGraph* graph, const std::string& nodeName, Args&&... args)
    {
        static_assert(T::GetRequiredChildrenCount() > 0);
        SceneNode* n = new SceneNode();
        n->m_SceneGraph = graph;
        n->m_NodeName = nodeName;
        n->m_NodeType = T::GetNodeType();
        n->m_Children.reserve(T::GetRequiredChildrenCount());
        std::array<const AnalyticScalarField*, T::GetRequiredChildrenCount()> arrayFields; 

        for (size_t i = 0; i < T::GetRequiredChildrenCount(); i++)
        {
            n->m_Children.emplace_back(CreateLeaf<VoidImplicit>(graph, "Empty"));
            n->m_Children.back()->setParent(n);
            arrayFields[i] = n->m_Children.back()->getImplicit();
        }

        auto tupleFields = array_to_tuple<const AnalyticScalarField*, T::GetRequiredChildrenCount()>(arrayFields);
        auto tupleArgs = std::tuple_cat(tupleFields, std::make_tuple(std::forward<Args>(args)...));
        
        n->m_Implicit = std::unique_ptr<T>(new T(std::make_from_tuple<T>(tupleArgs)));
        n->m_IsLeaf = false;

        return std::unique_ptr<SceneNode>(n);
    }

    // Getters

    inline SceneGraph* getSceneGraph() { return m_SceneGraph; }

    inline const std::string& getNodeName() const { return m_NodeName; }

    inline std::string& getNodeName() { return m_NodeName; }

    inline NodeType::Type getNodeType() const { return m_NodeType; }

    inline const AnalyticScalarField* getImplicit() const { return m_Implicit.get(); }

    inline AnalyticScalarField* getImplicit() { return m_Implicit.get(); }

    inline const Container& getChildren() const { return m_Children; }

    inline Container& getChildren() { return m_Children; }

    inline const SceneNode* getChild(size_t i) const { return m_Children.at(i).get(); }

    inline SceneNode* getChild(size_t i) { return m_Children.at(i).get(); }

    inline size_t getChildrenCount() const { return m_Children.size(); }

    inline bool isLeaf() const { return m_IsLeaf; }

    inline bool isNode() const { return !m_IsLeaf; }

    inline SceneNode* getParent() { return m_Parent; }

    size_t getFieldContaining(const SceneNode* node) const;

    // Setters

    inline void setNodeName(const std::string& nodeName) { m_NodeName = nodeName; }
    
    SceneNode* setChildNode(size_t field, std::unique_ptr<SceneNode>&& node);

    SceneNode* setNode(std::unique_ptr<SceneNode>&& node);

    inline void setSelected(bool selected) { m_Selected = selected; }

    // Processing
    
    float value(const glm::vec3& p) const;

    // GUI

    void onImgui();

private:
    SceneNode()
    {
        m_NodeName.reserve(50);
    }

    inline void setParent(SceneNode* parent) { m_Parent = parent; }

private:
    SceneGraph* m_SceneGraph = nullptr;
    SceneNode* m_Parent = nullptr;
    std::string m_NodeName;
    NodeType::Type m_NodeType = NodeType::Type::InvalidNode;
    std::unique_ptr<AnalyticScalarField> m_Implicit;
    Container m_Children;
    bool m_IsLeaf = true;
    bool m_Selected = false;
};
