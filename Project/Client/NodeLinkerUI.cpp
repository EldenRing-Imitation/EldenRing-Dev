#include "pch.h"
#include "NodeLinkerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CBehaviorTree.h>

#include "TreeUI.h"

NodeLinkerUI::NodeLinkerUI()
	: UI("##NodeLinker")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
{
	SetName("NodeLinker");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("NodeLinkerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&NodeLinkerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&NodeLinkerUI::DragDrop);
	m_Tree->SetDragDropID("BTNode");

	AddChildUI(m_Tree);
}

NodeLinkerUI::~NodeLinkerUI()
{
}

void NodeLinkerUI::tick()
{
}

int NodeLinkerUI::render_update()
{
	return 0;
}

void NodeLinkerUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// ������ ��Ʈ���κ��� �Ļ��� ��� ��� �޾ƿ�
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void NodeLinkerUI::SetTargetObject(CGameObject* _Target)
{
	// ClearTargetResource()

	// �� Ÿ��
	CComponent* Com = _Target->BehaviorTree();
	if (Com)
		m_RootNode = (DWORD_PTR)_Target->BehaviorTree()->GetRootNode();

	else
		m_RootNode = 0;
	
	ResetNodeLinker();
}

void NodeLinkerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
}

BTNode* NodeLinkerUI::GetSelectedNode()
{
	return nullptr;
}

void NodeLinkerUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
{
	// ��带 Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
	TreeNode* pNode = m_Tree->AddItem(string(_Node->GetNodeName().begin(), _Node->GetNodeName().end())
									, (DWORD_PTR)_Node
									, _ParentNode);

	// ����� �ڽĳ����� ���� ��带 �θ���ؼ� �� ������ �ٽ� �־��ش�.
	BTNode* ChildNode = _Node->GetChildNode();
	if (ChildNode)
		AddNode(ChildNode, pNode);

	list<BTNode*> ChildNodes = _Node->GetChildNodes();
	if (!(ChildNodes.empty()))
	{
		list<BTNode*>::iterator iter = ChildNodes.begin();

		for (; iter != ChildNodes.end(); ++iter)
		{
			AddNode(*iter, pNode);
		}
	}
}

void NodeLinkerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
}

