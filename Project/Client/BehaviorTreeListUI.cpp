#include "pch.h"
#include "BehaviorTreeListUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"
#include "NodeInfoUI.h"

#include <Engine/CBehaviorTree.h>

#include "TreeUI.h"

BehaviorTreeListUI::BehaviorTreeListUI()
	: UI("##BehaviorTreeList")
	, m_Tree(nullptr)
	, m_dwSelectedData(0)
	, m_RootNode(0)
	, m_NodeInfo(nullptr)
{
	SetName("BehaviorTreeList");
	
	m_Tree = new TreeUI;
	m_Tree->SetName("BehaviorTree");
	
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(true);
	m_Tree->ShowArrowBtn(true);
	m_Tree->ShowGroupIdx(true);
	m_Tree->SetFlags(ImGuiTreeNodeFlags_CollapsingHeader);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&BehaviorTreeListUI::DragDrop);
	m_Tree->AddDynamic_SwapFront(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SwapNodeFront);
	m_Tree->AddDynamic_SwapBack(this, (UI_DELEGATE_1)&BehaviorTreeListUI::SwapNodeBack);

	m_Tree->SetDragDropID("BTNode");

	m_NodeInfo = new NodeInfoUI;
	// m_NodeInfo->SetActive(true);

	// AddChildUI(m_Tree);
	// AddChildUI(m_NodeInfo);
}

BehaviorTreeListUI::~BehaviorTreeListUI()
{
	delete m_Tree;
	delete m_NodeInfo;
}

void BehaviorTreeListUI::tick()
{
	// m_NodeInfo->SetActive(true);
	m_Tree->tick();
	m_NodeInfo->tick();
}

int BehaviorTreeListUI::render_update()
{
	ImVec2 size = ImGui::GetWindowSize();
	string strFullName = m_Tree->GetName() + m_Tree->GetID();
	{
		ImGui::GetWindowSize();
		ImGui::BeginChild(strFullName.c_str(), ImVec2(size.x - 225, -ImGui::GetFrameHeightWithSpacing()));
		ImGui::BeginGroup();
		m_Tree->render_update();
		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::SameLine();
		{
			ImGui::BeginGroup();
			strFullName = m_NodeInfo->GetName() + m_NodeInfo->GetID();
			ImGui::BeginChild(strFullName.c_str(), ImVec2(200, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
			m_NodeInfo->render_update();
			ImGui::EndChild();
			if (ImGui::Button("Save")) {}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {}
			ImGui::EndGroup();
		}
	}

	return 0;
}

void BehaviorTreeListUI::ResetNodeLinker()
{
	SetSize(300.f, 500.f);

	// Tree Clear
	m_Tree->Clear();

	// ������ ��Ʈ���κ��� �Ļ��� ��� ��� �޾ƿ�
	if(m_RootNode)
		AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SetTargetObject(CGameObject* _Target)
{
	// ClearTargetResource()

	// �� Ÿ��
	CComponent* Com = _Target->BehaviorTree();
	if (Com)
	{
		m_RootNode = (DWORD_PTR)_Target->BehaviorTree()->GetRootNode();
	}

	else
	{
		m_RootNode = 0;
		m_NodeInfo->SetTargetNode(nullptr);
	}
	
	ResetNodeLinker();
}

void BehaviorTreeListUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	m_NodeInfo->SetTargetNode((BTNode*)pSelectedNode->GetData());
}

BTNode* BehaviorTreeListUI::GetSelectedNode()
{
	return m_NodeInfo->GetTargetNode();
}

void BehaviorTreeListUI::AddNode(BTNode* _Node, TreeNode* _ParentNode)
{

	// ��带 Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
	TreeNode* pNode = m_Tree->AddItem(string(_Node->GetNodeName().begin(), _Node->GetNodeName().end())
									, (DWORD_PTR)_Node
									, _ParentNode);


	switch (_Node->GetNodeType())
	{
	case BT_ROOT:
		pNode->SetNodeColor(0);
		break;
	case BT_COMPOSITE:
		pNode->SetNodeColor(4);
		break;
	case BT_TASK:
		pNode->SetNodeColor(5);
		break;
	}
	

	int	NodeGroupIdx = 1;
	
	if (_ParentNode != nullptr)
	{
		NodeGroupIdx = _ParentNode->GetChildNodeSize() + 1;
		_ParentNode->SetChildNodeSize(NodeGroupIdx);
	}

	pNode->SetGroupIdx(NodeGroupIdx);


	list<BTNode*> ChildNodes = _Node->GetChild();
	if (!(ChildNodes.empty()))
	{
		list<BTNode*>::iterator iter = ChildNodes.begin();

		for (; iter != ChildNodes.end(); ++iter)
		{
			AddNode(*iter, pNode);
			NodeGroupIdx++;
		}
	}
}

void BehaviorTreeListUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
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
		if (pDropBT->IsAncestorNode(pDragBT))
			return;
	}
	else if (nullptr == pDropBT)
		return;

	// ��ӳ�忡 �ڽ��� ���� �� �巡�׳�尡 task�ΰ��
	if (0 < pDropBT->GetChildCnt() && pDragBT->GetNodeType() == BT_TASK)
		return;

	pDropBT->AddChild(pDragBT);

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SwapNodeFront(DWORD_PTR _pNode)
{
	TreeNode* pNode = (TreeNode*)_pNode;
	BTNode* pBTNode = (BTNode*)pNode->GetData();

	pBTNode->SwapFront();

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}

void BehaviorTreeListUI::SwapNodeBack(DWORD_PTR _pNode)
{
	TreeNode* pNode = (TreeNode*)_pNode;
	BTNode* pBTNode = (BTNode*)pNode->GetData();

	pBTNode->SwapBack();

	// Tree Update
	m_Tree->Clear();
	AddNode((BTNode*)m_RootNode, nullptr);
}