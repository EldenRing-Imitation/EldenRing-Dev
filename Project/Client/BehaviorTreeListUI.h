#pragma once
#include "UI.h"

class BTNode;
class TreeUI;
class NodeInfoUI;
class BlackBoardListUI;

class BehaviorTreeListUI :
    public UI
{
private:
    TreeUI*     m_Tree;
    DWORD_PTR   m_dwSelectedData;
    DWORD_PTR   m_RootNode;

    NodeInfoUI* m_NodeInfo;
    BlackBoardListUI* m_BBList;

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    void OpenBlackBoardListUI();
    void SetRootNode(BTNode* _Node) { m_RootNode = (DWORD_PTR)_Node; }
    void ResetNodeLinker();
    void SetTargetObject(CGameObject* _Target);
    void SetTargetToInspector(DWORD_PTR _SelectedNode);
    BTNode* GetSelectedNode();

    // Ʈ���� ���µ� ���, �Է����� ���� �����Ϳ� ������ ��带 ���û��·� �д�.
    void SetSelectedNodeData(DWORD_PTR _data)
    {
        m_dwSelectedData = _data;
    }

private:
    void AddNode(BTNode* _Node, TreeNode* _ParentNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);

    void SwapNodeFront(DWORD_PTR _pNode);
    void SwapNodeBack(DWORD_PTR _pNode);

public:
    BehaviorTreeListUI();
    virtual ~BehaviorTreeListUI();
};

