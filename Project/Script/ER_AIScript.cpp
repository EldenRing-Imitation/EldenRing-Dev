#include "pch.h"
#include "ER_AIScript.h"

#include "ER_AIScript_Nodes.cpp""

ER_AIScript::ER_AIScript(SCRIPT_TYPE _type)
	:CScript((UINT)_type)
	, m_Character(nullptr)
	, m_pActionScript(nullptr)
	, m_pBehaviorTree(nullptr)
{
}

ER_AIScript::~ER_AIScript()
{
}

void ER_AIScript::SetBattleStartTime(float _fTime)
{
	if (m_pBehaviorTree)
	{
		BB* BlackBoard = m_pBehaviorTree->GetRootNode()->GetBlackBoard();
		BlackBoard->AddBBData(L"BattleTime", _fTime);
	}
}

Selector_Node* ER_AIScript::SetERCharacterAIBase()
{
	// �ൿƮ�� Component ����
	m_pBehaviorTree = onew(CBehaviorTree);
	GetOwner()->AddComponent(m_pBehaviorTree);
	m_pBehaviorTree->SetOwner(GetOwner());


	// =========================== [   1   ] ===========================
	// ��Ʈ �Ҵ�
	Root_Node* root = m_pBehaviorTree->GetRootNode();

	SetBattleStartTime(60.f);

	// [ ���� �Ǵ� ]
	// Seq_CheckArrive : ĳ���� ���� �Ǵ� ������
	Sequence_Node* Seq_CheckArrive = root->AddChild<Sequence_Node>();
	// Con_IsArrive : ĳ���� ���� �Ǵ� ���
	Seq_CheckArrive->AddChild<Con_IsArrive>();

	// Sel_ExecuteAICommand : AI ��� ���� ������
	Selector_Node* Sel_ExecuteAICommand = root->AddChild<Selector_Node>();

	// Seq_CheckGameTime : GameTime ���� ������
	Sequence_Node* Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();

	// Con_IsBeforeTime : ������ �ð��� ������ ������ �Ǵ�
	Seq_CheckGameTime->AddChild <Con_IsBeforeTime>();

	// Sel_ExploreCommand : Ž��(����) ���� ��� ������
	Selector_Node* Sel_ExploreCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	// =========================== [   2   ] ===========================
	// ���� ->  Sel_ExploreCommand ���� �켱���� ù��°
	// 



	// =========================== [   3   ] ===========================








	// Sel_BattleCommand : �������� ��� ������
	Selector_Node* Sel_BattleCommand = Sel_ExecuteAICommand->AddChild<Selector_Node>();

	return Sel_BattleCommand;
}