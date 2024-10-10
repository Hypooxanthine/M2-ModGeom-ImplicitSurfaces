#pragma once

class SceneNode;

class NodeEditor
{
public:
    NodeEditor() = default;
    NodeEditor(SceneNode* node);

    inline const SceneNode* getNode() const { return m_Node; }

    inline SceneNode* getNode() { return m_Node; }

    inline void disable() { m_Node = nullptr; }

    void onImgui();

protected:
    virtual void onImgui_Impl() = 0;

private:
    SceneNode* m_Node = nullptr;
};