#include "pch.h"
#include "ER_AIScript.h"

#include "ER_AIScript_Nodes.cpp"
#include "ER_ItemMgr.h"

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

void ER_AIScript::RegistAllItemBox(int _StartIdx)
{
	// ���� �ε������� ����� ��-> ó������ �����ε��� ������
	
	int iAreaIdx = _StartIdx;
	
	// ���ڷ� ���� �ε������� �����ؼ� ������ Ž���ϰ�, ó������ ���ư� �����ε��� ������ �۾�����
	do
	{
		// ���� ������ �����۹ڽ��� ���� �޾ƿ´�.
		vector<CGameObject*> AreaItemBox = ER_ItemMgr::GetInst()->GetItemBoxList(iAreaIdx);

		// �����۹ڽ� ������ �����ش�
		std::random_device rd;
		std::mt19937 gen(rd());

		std::shuffle(AreaItemBox.begin(), AreaItemBox.end(), gen);

		// ������� Ž�� �����۹ڽ� Queue�� ����־��ش�
		for (auto ItemBox : AreaItemBox)
		{
			m_queueItemBox.push(ItemBox);
		}

		// �ε��� �缳��
		// ������ ���� Idx�� �����ߴٸ� ó������ ����
		if (iAreaIdx + 1 == (UINT)LUMIAISLAND::END)
			iAreaIdx = (UINT)LUMIAISLAND::ARCHERY; // 0 ������
		else
			iAreaIdx++;	// ������������ �ε��� ����

	} while (iAreaIdx != _StartIdx);


}


CGameObject* ER_AIScript::GetNextItemBox()
{
	CGameObject* ItemBox = nullptr;
	
	if (!m_queueItemBox.empty())
	{
		m_queueItemBox.pop();
		ItemBox = m_queueItemBox.front();
	}

	return ItemBox;
}


Selector_Node* ER_AIScript::SetERCharacterAIBase()
{
	// �ൿƮ�� Component ����
	m_pBehaviorTree = onew(CBehaviorTree);
	GetOwner()->AddComponent(m_pBehaviorTree);
	m_pBehaviorTree->SetOwner(GetOwner());

	// ���� BlackBoard ����
	BB* BlackBoard = m_pBehaviorTree->GetRootNode()->GetBlackBoard();
	int CraftItemID = -1;
	CGameObject* ItemBox = m_queueItemBox.front();
	BlackBoard->AddBBData(L"CraftItemID", CraftItemID);
	BlackBoard->AddBBData(L"ExploreItemBox", ItemBox);
	BlackBoard->AddBBData(L"CurActiveSkill", -1);
	BlackBoard->AddBBData(L"CheckTime", 0.f);

	// =========================== [   1   ] ===========================
	// ��Ʈ �Ҵ�
	Root_Node* root = m_pBehaviorTree->GetRootNode();

	// [ ���� �Ǵ� ]
	// Seq_CheckArrive : ĳ���� ���� �Ǵ� ������
	Selector_Node* Sel_CheckArrive = root->AddChild<Selector_Node>();
	// Con_IsDEADState : ĳ���Ͱ� �����������
	Sel_CheckArrive->AddChild<Con_IsDEADState>();

	// Sel_ExecuteAICommand : AI ��� ���� ������
	Selector_Node* Sel_ExecuteAICommand = Sel_CheckArrive->AddChild<Selector_Node>();

	// Seq_CheckGameTime : GameTime ���� ������
	Sequence_Node* Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();

	// Con_IsBeforeTime : ������ �ð��� ������ ������ �Ǵ�
	Seq_CheckGameTime->AddChild <Con_IsBeforeTime>();

	// Sel_ExploreCommand : Ž��(����) ���� ��� ������
	Selector_Node* Sel_ExploreCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	// =========================== [   ��ų����   ] ===========================
	Sequence_Node* Seq_SkillUp = Sel_ExploreCommand->AddChild<Sequence_Node>();
	
	Seq_SkillUp->AddChild<Con_IsHaveSkillPoint>();	// ��ų ����Ʈ �˻�
	Seq_SkillUp->AddChild<Act_SkillLevelUp>();		// ��ų ����Ʈ ����

	// =========================== [   ����   ] ===========================
	Selector_Node* Sel_Craft = Sel_ExploreCommand->AddChild<Selector_Node>();
	// ���� �������� ���������� : �������̶�� �̹� ����� �������Ѵ�.
	Sel_Craft->AddChild<Con_IsCRAFTState>();
	
	// �������� �ƴ϶�� ������ ���� ����
	Sequence_Node* Seq_Craft = Sel_Craft->AddChild<Sequence_Node>();

	// ���� ������ �������� �ִ��� : �ִٸ� �����忡 ������ ������ ���̵� �����ϰ� �� �������� ����
	Seq_Craft->AddChild<Con_IsCanCraftItem>();
	Seq_Craft->AddChild<Act_CraftItem>();

	// =========================== [   �Ĺ�   ] ===========================
	Selector_Node* Sel_Farming = Sel_ExploreCommand->AddChild<Selector_Node>();

	// �Ĺֻ������� Ȯ����, �Ĺ��۾� ����. ���� �� �����۹ڽ�Ž�� ť ����
	Sequence_Node* Seq_Farming = Sel_Farming->AddChild<Sequence_Node>();
	Seq_Farming->AddChild<Con_IsFARMINGState>();
	Seq_Farming->AddChild<Act_Farming>();
	Seq_Farming->AddChild<Act_FarmingEnd>();

	// �Ĺֻ��°� �ƴ϶�� Ž������ �����۹ڽ��� �̵��Ѵ�
	Sequence_Node* Seq_Explore = Sel_Farming->AddChild<Sequence_Node>();
	Seq_Explore->AddChild<Con_hasNextBox>();
	Seq_Explore->AddChild<Act_FarmingCommand>();



	// Sel_BattleCommand : �������� ��� ������
	Seq_CheckGameTime = Sel_ExecuteAICommand->AddChild<Sequence_Node>();
	Seq_CheckGameTime->AddChild<Con_IsAfterTime>();
	Selector_Node* Sel_BattleCommand = Seq_CheckGameTime->AddChild<Selector_Node>();

	return Sel_BattleCommand;
}