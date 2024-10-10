#pragma once

#include <string>
#include <vector>

#include "Implicits/implicits.h"

class SceneNodeIterator;

class SceneNode
{
public:
    using Container = std::vector<SceneNode*>;
public:
    SceneNode() = default;
    ~SceneNode() = default;

    // Getters

    inline const std::string& getNodeName() const { return m_NodeName; }

    inline AnalyticScalarField* getImplicit() const { return m_Implicit; }

    inline const Container& getChildren() const { return m_Children; }

    inline size_t getChildrenCount() const { return m_Children.size(); }

    // Setters

    inline void setNodeName(const std::string& nodeName) { m_NodeName = nodeName; }

    inline void setImplicit(AnalyticScalarField* implicit) { m_Implicit = implicit; }

    inline void addChild(SceneNode* child) { m_Children.push_back(child); }

    // Iteration

    inline SceneNodeIterator begin();

    inline SceneNodeIterator end();

private:
    std::string m_NodeName;
    AnalyticScalarField* m_Implicit;
    Container m_Children;
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
        m_NodeStack.emplace(m_NodeStack.top()->getChildren().at(index));
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