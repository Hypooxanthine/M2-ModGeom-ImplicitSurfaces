#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Implicits/implicits.h"
#include "Implicits/Primitives/VoidImplicit.h"

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

class SceneNodeIterator;

class SceneNode
{
public:
    using Container = std::vector<SceneNode>;
    using iterator = SceneNodeIterator;
public:

    SceneNode(SceneNode&& other) = default;

    SceneNode& operator=(SceneNode&& other) = default;

    ~SceneNode() = default;

    // Static
    /**
     * Only for leaves, analytic scalar field must be a primitive
     */
    template <typename T, typename... Args>
    static SceneNode CreateLeaf(SceneGraph* graph, const std::string& nodeName, Args&&... args)
    {
        static_assert(T::GetRequiredChildrenCount() == 0);
        SceneNode n;
        n.m_SceneGraph = graph;
        n.m_NodeName = nodeName;
        n.m_Implicit = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        n.m_IsLeaf = true;

        return n;
    }

    template <typename T, typename... Args>
    static SceneNode CreateNode(SceneGraph* graph, const std::string& nodeName, Args&&... args)
    {
        static_assert(T::GetRequiredChildrenCount() > 0);
        SceneNode n;
        n.m_SceneGraph = graph;
        n.m_NodeName = nodeName;
        n.m_Children.reserve(T::GetRequiredChildrenCount());
        std::array<const AnalyticScalarField*, T::GetRequiredChildrenCount()> arrayFields; 

        for (size_t i = 0; i < T::GetRequiredChildrenCount(); i++)
        {
            n.m_Children.emplace_back(CreateLeaf<VoidImplicit>(graph, "Empty"));
            arrayFields[i] = &n.m_Children.back().getImplicit();
        }

        auto tupleFields = array_to_tuple<const AnalyticScalarField*, T::GetRequiredChildrenCount()>(arrayFields);
        auto tupleArgs = std::tuple_cat(std::make_tuple(std::forward<Args>(args)...), tupleFields);
        
        n.m_Implicit = std::unique_ptr<T>(new T(std::make_from_tuple<T>(tupleArgs)));
        n.m_IsLeaf = false;

        return n;
    }

    // Getters

    inline const std::string& getNodeName() const { return m_NodeName; }

    inline std::string& getNodeName() { return m_NodeName; }

    inline const AnalyticScalarField& getImplicit() const { return *m_Implicit; }

    inline const SceneNode& getChild(size_t i) const { return m_Children.at(i); }

    inline SceneNode& getChild(size_t i) { return m_Children.at(i); }

    inline size_t getChildrenCount() const { return m_Children.size(); }

    inline bool isLeaf() const { return m_IsLeaf; }

    inline bool isNode() const { return !m_IsLeaf; }

    // Setters

    inline void setNodeName(const std::string& nodeName) { m_NodeName = nodeName; }
    
    void setChildNode(size_t field, SceneNode&& node);

    inline void setSelected(bool selected) { m_Selected = selected; }

    // Processing
    
    float value(const glm::vec3& p) const;

    // Iteration

    inline iterator begin();

    inline iterator end();

    // GUI

    void onImgui();

private:
    SceneNode()
    {
        m_NodeName.reserve(50);
    }

private:
    SceneGraph* m_SceneGraph = nullptr;
    std::string m_NodeName;
    std::unique_ptr<AnalyticScalarField> m_Implicit;
    Container m_Children;
    bool m_IsLeaf = true;
    bool m_Selected = false;
};



#include <stack>

class SceneNodeIterator
{
private:
    SceneNodeIterator() = default;
public:
    SceneNodeIterator(SceneNode* root)
        : m_Root(root)
    {
        m_ChildIndexStack.push(0);
        m_NodeStack.push(root);
    }

    static SceneNodeIterator MakeEnd(SceneNode* root)
    {
        SceneNodeIterator it;
        it.m_Root = root;

        return it;
    }

    SceneNode& operator*()
    {
        return *m_NodeStack.top();
    }

    SceneNode& operator->()
    {
        return *m_NodeStack.top();
    }

    SceneNodeIterator& operator++()
    {
        while (!m_ChildIndexStack.empty() && m_NodeStack.top()->getChildrenCount() <= m_ChildIndexStack.top())
        {
            m_NodeStack.pop();
            m_ChildIndexStack.pop();
        }

        if (m_ChildIndexStack.empty())
            return *this;
        
        auto index = m_ChildIndexStack.top();
        // Update the index of the next child to be visited on this layer
        ++m_ChildIndexStack.top();

        m_ChildIndexStack.emplace(index);
        m_NodeStack.emplace(&m_NodeStack.top()->getChild(index));

        return *this;
    }

    SceneNodeIterator operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const SceneNodeIterator& other) const
    {
        // Since each node is visited once,
        // we can just check if roots are the same and the current nodes are the same.
        return m_Root == other.m_Root && m_NodeStack.top() == other.m_NodeStack.top();
    }

    bool operator!=(const SceneNodeIterator& other) const
    {
        return !(*this == other);
    }

private:
    std::stack<size_t> m_ChildIndexStack;
    std::stack<SceneNode*> m_NodeStack;

    const SceneNode* m_Root = nullptr; // Saving the root for faster == and !=
};

SceneNodeIterator SceneNode::begin()
{
    return SceneNodeIterator(this);
}

SceneNodeIterator SceneNode::end()
{
    return SceneNodeIterator::MakeEnd(this);
}