#pragma once

#include <string>
#include <vector>

#include "Implicits/implicits.h"

class SceneNode
{
public:
    SceneNode() = default;
    ~SceneNode() = default;

    // Getters

    inline const std::string& getNodeName() const { return m_NodeName; }

    inline AnalyticScalarField* getImplicit() const { return m_Implicit; }

    inline const std::vector<SceneNode*>& getChildren() const { return m_Children; }

    inline size_t getChildrenCount() const { return m_Children.size(); }

    // Setters

    inline void setNodeName(const std::string& nodeName) { m_NodeName = nodeName; }

    inline void setImplicit(AnalyticScalarField* implicit) { m_Implicit = implicit; }

    inline void addChild(SceneNode* child) { m_Children.push_back(child); }

private:
    std::string m_NodeName;
    AnalyticScalarField* m_Implicit;
    std::vector<SceneNode*> m_Children;
};
