#include "pch.h"
#include "BehaviorTreeUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CBehaviorTree.h>

#include "TreeUI.h"

BehaviorTreeUI::BehaviorTreeUI()
	: UI("##BehaviorTree")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
{
	SetName("BehaviorTree");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("BehaviorTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);
	m_Tree->ShowArrowBtn(true);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&BehaviorTreeUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&BehaviorTreeUI::DragDrop);
	m_Tree->SetDragDropID("BTNode");

	AddChildUI(m_Tree);
}

BehaviorTreeUI::~BehaviorTreeUI()
{
}

void BehaviorTreeUI::tick()
{
}

int BehaviorTreeUI::render_update()
{
	return 0;
}

void BehaviorTreeUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// ������ ��Ʈ���κ��� �Ļ��� ��� ��� �޾ƿ�
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeUI::SetTargetObject(CGameObject* _Target)
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

void BehaviorTreeUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
}

BTNode* BehaviorTreeUI::GetSelectedNode()
{
	return nullptr;
}

void BehaviorTreeUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
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

void BehaviorTreeUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;

	BTNode* pDragBT = (BTNode*)pDragNode->GetData();
	BTNode* pDropBT = nullptr;
	if (nullptr != pDropNode)
	{
		pDropBT = (BTNode*)pDropNode->GetData();
	}

	// �ڽ����� �� ������Ʈ�� ������ ������Ʈ�� ����(�θ����) �� �ϳ����, 
	// AddChild ó������ �ʴ´�.
	if (nullptr != pDropBT)
	{
		if (pDropBT->IsAncestor(pDragBT))
			return;
	}

	// �ൿƮ�� ��� ����
	if ((pDragBT->GetParentNode() == pDropBT)
		|| (NODETYPE::COMPOSITE != pDropBT->GetNodeType())
		|| (NODETYPE::ROOT == pDropBT->GetNodeType()))
		return;

	Composite_Node* ParentNode = dynamic_cast<Composite_Node*>(pDropBT);
	pDragBT->DisconnectFromParent();

	ParentNode->AddChildNode(pDragBT);

	// AddChild
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}

