#include "pch.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_struct.h"
#include <FontEngine\FW1FontWrapper.h>
#include "ER_DataScript_Item.h"
#include "ER_ItemMgr.h"
#include "ER_GameSystem.h"
#include "ER_PlayerScript.h"

ER_DataScript_Character::ER_DataScript_Character()
	: CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
	, m_STDStats{}
	, m_RootItem{}
	, bCoolDownCheat(false)
	, bInvincibleCheat(false)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;
}

ER_DataScript_Character::ER_DataScript_Character(const ER_DataScript_Character& _origin)
	: m_Stats(nullptr)
	, m_StatusEffect(nullptr)
	, m_strKey(_origin.m_strKey)
	, m_strName(_origin.m_strName)
	, m_STDStats(_origin.m_STDStats)
	, m_PortraitTex(_origin.m_PortraitTex)
	, m_FullTax(_origin.m_FullTax)
	, m_MapTex(_origin.m_MapTex)
	, m_bGameDead(false)
	, m_bOutofContorl(false)
	, m_Equipment{}
	, m_Inventory{}
	, CScript((UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER)
	, m_fSPRegenTime(0.f)
	, m_SkillPoint(0)
	, m_RootItem{}
	, bCoolDownCheat(false)
	, bInvincibleCheat(false)
{
	m_Stats = new tIngame_Stats;
	m_StatusEffect = new tStatus_Effect;
	m_LevelUpSound = CResMgr::GetInst()->FindRes<CSound>(L"effect_levelup.wav").Get();

	for (int i = 0; i < _origin.m_SkillList.size(); ++i)
	{
		tSkill_Info* tmp = new tSkill_Info;
		*tmp = *_origin.m_SkillList[i];

		m_SkillList.push_back(tmp);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_RootItem[i] = _origin.m_RootItem[i];
	}

	m_IngredientList = _origin.m_IngredientList;
	m_NeedFarmingItems = _origin.m_NeedFarmingItems;
}

ER_DataScript_Character::~ER_DataScript_Character()
{
	if (m_Stats)
		delete m_Stats;

	if (m_StatusEffect)
		delete m_StatusEffect;

	Safe_Del_Vec(m_SkillList);
	Safe_Del_Array(m_Equipment);
	Safe_Del_Array(m_Inventory);

	// �κ��丮, ���� ������ delete �۾�
}

void ER_DataScript_Character::StatusUpdate()
{
	ER_Ingame_Stats Updatetmp = *m_Stats;
	UINT Level = Updatetmp.iLevel - 1;

	// ����,����ġ,HP,SP �� �������� �ʴ´�.
	// 1. �⺻ �������ͽ�
	Updatetmp.iAttackPower = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.iDefense = m_STDStats.iDefense + (Level * m_STDStats.iDefensePerLevel);
	Updatetmp.iMaxHP = m_STDStats.iMaxHP + (Level * m_STDStats.iMaxHPPerLevel);
	Updatetmp.fHPRegen = m_STDStats.fHPRegen + (Level * m_STDStats.fHPRegenPerLevel);
	Updatetmp.iMaxSP = m_STDStats.iMaxSP + (Level * m_STDStats.iMaxSPPerLevel);
	Updatetmp.fSPRegen = m_STDStats.fSPRegen + (Level * m_STDStats.fSPRegenPerLevel);
	Updatetmp.fAttackSpeed = m_STDStats.fAttackSpeed + m_STDStats.fWpAtkSpd;
	Updatetmp.fCriticalStrikeChance = m_STDStats.fCriticalStrikeChance;
	Updatetmp.fMovementSpeed = m_STDStats.fMovementSpeed;
	Updatetmp.fVisionRange = m_STDStats.fVisionRange;
	Updatetmp.iSkillAmplification = m_STDStats.iAttackPower + (Level * m_STDStats.iAttackPowerPerLevel);
	Updatetmp.fAtkRange = m_STDStats.fWpAtkRange;
	Updatetmp.fCriticalStrikeDamage = 0;
	Updatetmp.fCooldownReduction = 0;
	Updatetmp.fOmnisyphon = 0;
	Updatetmp.iSkillAmplification = 0;

	// 2. ������ �߰� �������ͽ�
	for (int i = 0; i < (UINT)ER_ITEM_SLOT::END; ++i)
	{
		// �������� �����Ǿ����� �ʴ�
		if (m_Equipment[i] == nullptr)
			continue;

		// ������ ������ ���� ������Ʈ
		tItem_Stats Itemtmp = GETITEMSTATS(m_Equipment[i]);

		Updatetmp.iAttackPower += Itemtmp.iAttackPower + (Level * Itemtmp.iAttackPowerPerLevel);
		Updatetmp.iDefense += Itemtmp.iDefense;
		Updatetmp.iMaxHP += Itemtmp.iMaxHP + (Level * Itemtmp.iMaxHPPerLevel);
		Updatetmp.fHPRegen += Itemtmp.fHPRegen + (Level * Itemtmp.fHPRegenPerLevel);
		Updatetmp.iMaxSP += Itemtmp.iMaxSP + (Level * Itemtmp.iMaxSPPerLevel);
		Updatetmp.fSPRegen += Itemtmp.fSPRegen + (Level * Itemtmp.fSPRegenPerLevel);
		Updatetmp.fAttackSpeed += Itemtmp.fAttackSpeed;
		Updatetmp.fCriticalStrikeChance += Itemtmp.fCriticalStrikeChance;
		Updatetmp.fCriticalStrikeDamage += Itemtmp.fCriticalStrikeDamage;
		Updatetmp.fMovementSpeed += Itemtmp.fMovementSpeed;
		Updatetmp.fVisionRange += Itemtmp.fVisionRange;
		Updatetmp.fCooldownReduction += Itemtmp.fCooldownReduction;
		Updatetmp.fOmnisyphon += Itemtmp.fOmnisyphon;
		Updatetmp.iSkillAmplification += Itemtmp.iSkillAmplification + (Level * Itemtmp.iSkillAmplificationPerLevel);
	}

	// �������� �ݿ�
	*m_Stats = Updatetmp;
}

void ER_DataScript_Character::HPRegen(float _magnification)
{
	// ȸ���� ȸ�� �� HP��
	float HPRegen = m_Stats->iHP + (m_Stats->fHPRegen * _magnification);

	// HP �ڿ� ȸ��, �ִ� HP�� �ִ�HP�� ����
	m_Stats->iHP = m_Stats->iMaxHP < (int)HPRegen ? m_Stats->iMaxHP : (int)HPRegen;
}

void ER_DataScript_Character::SPRegen(float _magnification)
{
	// ȸ���� ȸ�� �� HP��
	float SPRegen = m_Stats->iSP + (m_Stats->fSPRegen * _magnification);

	// HP �ڿ� ȸ��, �ִ� HP�� �ִ�HP�� ����
	m_Stats->iSP = m_Stats->iMaxSP < (int)SPRegen ? m_Stats->iMaxSP : (int)SPRegen;
}

void ER_DataScript_Character::LevelUP()
{
	// �ִ뷹�� 18 ������ ���
	if (m_Stats->iLevel < 18)
	{
		m_Stats->iLevel++;	// Level 1 ����
		m_SkillPoint++;		// ��ų ����Ʈ 1 ����
	}

	// [ ����Ʈ ]
	// ������ ����Ʈ �� �ִϸ��̼� ���

	// ������ ȿ���� ���
	// �÷��̾� ĳ���� ȭ�� �ȿ� ������������ ���
	{
		Vec3 PlayerPos = ER_GameSystem::GetInst()->GetPlayerCharacter()->Transform()->GetRelativePos();
		Vec3 CharacterPos = Transform()->GetRelativePos();
		float ListenDist = Vec3::Distance(PlayerPos, CharacterPos);

		if (ListenDist < 18.f)
			m_LevelUpSound->Play(1, 0.5, true);
	}

	// ������ ���� �� ������
	int LevelUpDefHP = m_Stats->iMaxHP;
	int LevelUpDefSP = m_Stats->iMaxSP;

	// [ �������ͽ� ���� �ݿ� ]
	StatusUpdate();

	// ���� �� �ִ� HP/SP ������ ���̰���ŭ ���� HP / SP�� �ݿ��� ȸ�����ش�.

	LevelUpDefHP = m_Stats->iMaxHP - LevelUpDefHP;
	LevelUpDefSP = m_Stats->iMaxSP - LevelUpDefSP;

	m_Stats->iHP += LevelUpDefHP;
	m_Stats->iSP += LevelUpDefSP;
}

void ER_DataScript_Character::init()
{
	// [�ؽ��� �ε�]
	if (0 < m_strKey.length())
	{
		wstring PortraitTexKey = L"Char_Portrait_";		// portrait
		wstring FullTexKey = L"Char_Full_";				// Full
		wstring MapTexKey = L"Char_Map_";				// map

		PortraitTexKey += m_strKey + L".png";
		FullTexKey += m_strKey + L".png";
		MapTexKey += m_strKey + L".png";

		m_PortraitTex = CResMgr::GetInst()->FindRes<CTexture>(PortraitTexKey);
		m_FullTax = CResMgr::GetInst()->FindRes<CTexture>(FullTexKey);
		m_MapTex = CResMgr::GetInst()->FindRes<CTexture>(MapTexKey);
	}

	for (int i = 0; i < 5; ++i)
	{
		ER_ItemMgr::GetInst()->GetIngredient(m_RootItem[i], &m_IngredientList, &m_NeedFarmingItems);
	}
}

void ER_DataScript_Character::begin()
{
	// ĳ���� �ʱ�ɷ�ġ�� �޾ƿ� ���� 1�� �ʱ�ȭ
	m_Stats->Init_To_LevelOne(m_STDStats);

	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}

	StatusUpdate();

	UINT StartWeapon = 0;
	if (m_strKey == L"Aya")
	{
		StartWeapon = 20;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Hyunwoo")
	{
		StartWeapon = 18;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Jackie")
	{
		StartWeapon = 108;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Yuki")
	{
		StartWeapon = 9;
		m_SkillPoint = 1;
	}
	else if (m_strKey == L"Rio")
	{
		m_SkillList[(UINT)SKILLIDX::Q_1]->iSkillLevel = 1;
		m_SkillList[(UINT)SKILLIDX::Q_2]->iSkillLevel = 1;
		StartWeapon = 28;
	}

	m_Equipment[0] = ER_ItemMgr::GetInst()->GetItemObj(StartWeapon)->Clone();
	m_Equipment[0]->GetScript<ER_DataScript_Item>()->m_bEquiped = true;
}

void ER_DataScript_Character::tick()
{
	// [ �������ͽ� ���� ]
	// ����ġ Ȯ�� �� ������, ���� ����ġ�� ������ �ʿ� ����ġ�� ������� �ݺ��Ѵ�.
	while (m_Stats->iNeedExpForNextLevel <= m_Stats->iExp)
	{
		// ��������, �ʰ��� �ܿ� ����ġ�� ���� Exp�� �������ش�.
		m_Stats->iExp = m_Stats->iExp - m_Stats->iNeedExpForNextLevel;

		// ������ �Լ� ȣ��
		LevelUP();
	}

	// SP �ڿ� ȸ��
	m_fSPRegenTime += DT;
	if (0.5f <= m_fSPRegenTime)
	{
		SPRegen();
		m_fSPRegenTime -= 0.5f;
	}


	//  [ ��������� ��Ÿ�� ���� ]
	m_StatusEffect->ActionTiemUpdate(DT);


	// [ ��ų ��Ÿ�� ���� ]
	float CoolDownRatio = DT + (DT * m_Stats->fCooldownReduction);

	// Developer Func
	// [CoolTime Delete Mode]
	if (KEY_TAP(KEY::F3))
	{
		bCoolDownCheat = !bCoolDownCheat;
	}

	if (bCoolDownCheat)
	{
		m_Stats->iSP = m_Stats->iMaxSP;
		CoolDownRatio = 500.f;
	}

	// [EXP Increase Key]
	if (KEY_TAP(KEY::F2))
	{
		m_Stats->iExp += 100; // ������ �ʿ� ����ġ 100
	}

	// [ Invincible Mode ]
	if (KEY_TAP(KEY::F4))
	{
		ER_PlayerScript* IsPlayer = GetOwner()->GetScript<ER_PlayerScript>();
		
		if (nullptr != IsPlayer)
		{
			bInvincibleCheat = !bInvincibleCheat;
		}
	}

	if (bInvincibleCheat)
		m_Stats->iHP = m_Stats->iMaxHP;

	for (int i = 0; i < (UINT)SKILLIDX::SKILLMAXSIZE; ++i)
		m_SkillList[i]->SkillStatusUpdate(CoolDownRatio);
};

bool ER_DataScript_Character::SwapItem(CGameObject** _DragItem, CGameObject** _DropItem)
{
	ER_DataScript_Item* DragItem = nullptr;
	int DragItemType = -1;
	bool DragItemEquiped = false;

	ER_DataScript_Item* DropItem = nullptr;
	int DropItemType = -1;
	bool DropItemEquiped = false;

	if (*_DragItem)
	{
		DragItem = (*_DragItem)->GetScript<ER_DataScript_Item>();
		DragItemType = DragItem->m_eSlot;
		DragItemEquiped = DragItem->m_bEquiped;
	}
	if (*_DropItem)
	{
		DropItem = (*_DropItem)->GetScript<ER_DataScript_Item>();
		DropItemType = DropItem->m_eSlot;
		DropItemEquiped = DropItem->m_bEquiped;
	}

	// ���â -> �κ��丮
	// 1. ��ĭ���� �̵��ϴ°��
	// �׳� �̵�, �̵��� ������ ����Ÿ�� 2�α�ü
	// 2. �����۰� ��ü �ϴ°��
	// ������Ÿ�� ���� ��ü, ���� ����Ÿ�� ��ü

	// �κ��丮 -> ���â
	// 1. ��ĭ���� �̵��ϴ� ���
	// ������ Ÿ�� Ȯ��, ������ �ּҿ� ��񽽷� �ּҰ� ������ Ȯ��
	// 2. �����۰� ��ü�ϴ� ���
	// ������Ÿ�� ���� ��ü, ���� ����Ÿ�� ��ü

	// �κ��丮orItemBox ����
	// �׳� ��ü

	// ���â ->
	if (DragItemEquiped && !DropItemEquiped)
	{
		// ���� ������ ���� ���
		if (DragItemType == DropItemType)
		{
			// ������ ��ȯ
			CGameObject* tmp = (*_DropItem);
			(*_DropItem) = (*_DragItem);
			(*_DragItem) = tmp;

			// �������� ����
			(*_DragItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;
			(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;

			ItemInfoUpdate();
			StatusUpdate();
		}
		// -> NULLSlot
		else if (DropItemType == -1)
		{
			// -> ���â : �ٸ� ����Ÿ���� ���â�̹Ƿ� �Ű����� �ʴ´�.
			for (int i = 0; i < 5; ++i)
			{
				// ���â �ε��� Ȯ��
				if (_DropItem == &m_Equipment[i])
				{
					return false;
				}
			}

			// ������ ��ȯ
			CGameObject* tmp = (*_DropItem);
			(*_DropItem) = (*_DragItem);
			(*_DragItem) = tmp;

			(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;

			ItemInfoUpdate();
			StatusUpdate();
		}
	}
	// �κ��丮 or ItemBox ->
	else
	{
		// -> ���â
		for (int i = 0; i < 5; ++i)
		{
			// ���â �ε��� Ȯ��
			if (_DropItem == &m_Equipment[i])
			{
				// ������Ÿ�԰� ������ ��ġ�Ѵ�
				if (DragItemType == i)
				{
					// ������ ����ִ�
					if (DropItemType == -1)
					{
						CGameObject* tmp = (*_DropItem);
						(*_DropItem) = (*_DragItem);
						(*_DragItem) = tmp;

						(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;

						ItemInfoUpdate();
						StatusUpdate();
						return true;
					}
					else if (DragItemType == DropItemType)
					{
						// ������ ��ȯ
						CGameObject* tmp = (*_DropItem);
						(*_DropItem) = (*_DragItem);
						(*_DragItem) = tmp;

						// �������� ����
						(*_DragItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = false;
						(*_DropItem)->GetScript<ER_DataScript_Item>()->m_bEquiped = true;

						ItemInfoUpdate();
						StatusUpdate();
						return true;
					}
				}
				return false;
			}
		}

		// -> �κ��丮 or ������ �ڽ�

		// Inventory�� �ִ� �������̸� 
		// Drag�� ItemSlot�� �ƴѰ��
		CGameObject* tmp = (*_DropItem);
		(*_DropItem) = (*_DragItem);
		(*_DragItem) = tmp;

		ItemInfoUpdate();
	}

	return false;
}

void ER_DataScript_Character::AcquireItem(CGameObject** _BoxSlot)
{
	// �����ܽ��Կ� �������� �ִ��� ����
	if ((*_BoxSlot))
	{
		int Slotidx = -1;
		int slottype = 0;

		// ������ ��ũ��Ʈ ���� Ȯ�� ����ó��
		ER_DataScript_Item* Item = (*_BoxSlot)->GetScript<ER_DataScript_Item>();

		// ItemScript�� �����ϰ����� �ʴ� - �Լ�����
		if (!Item)
			return;

		int emptyslot = -1;
		// �κ��丮 �������� Ȯ��
		for (int i = 0; i < 10; ++i)
		{
			if (nullptr == m_Inventory[i])
			{
				emptyslot = i;
				break;
			}
		}

		// �κ��丮�� ���������� ����. - �Լ�����
		if (emptyslot == -1)
			return;

		// ����ִ� �κ��丮�� �־��ְ�, ���� ������ ����ش�.
		m_Inventory[emptyslot] = (*_BoxSlot);
		m_Inventory[emptyslot]->GetScript<ER_DataScript_Item>()->m_bEquiped = false;
		*_BoxSlot = nullptr;

		int ItemID = m_Inventory[emptyslot]->GetScript<ER_DataScript_Item>()->GetCode();

		// ������ �������� �ʿ� �Ĺ־������̾��ٸ� ����Ʈ�� �������ش�
		unordered_map<UINT, int>::iterator iter = m_NeedFarmingItems.find(ItemID);
		
		if (iter != m_NeedFarmingItems.end())
		{
			if (iter->second == 1)
				m_NeedFarmingItems.erase(iter);
			else
				iter->second--;
		}

		StatusUpdate();
		ItemInfoUpdate();
	}
}

void ER_DataScript_Character::ItemInfoUpdate()
{
	std::thread t1(&ER_DataScript_Character::ItemInfoUpdate, ItemInfoUpdate);
	t1.detach();

	lock_guard<mutex> lockGuard(m_mMutex);

	
	// ���۰��� ������ ������Ʈ
	vector<UINT> itemlist;

	for (int i = 0; i < 10; ++i)
	{
		if (i < 5 && m_Equipment[i])
			itemlist.push_back(m_Equipment[i]->GetScript<ER_DataScript_Item>()->m_eItemCode);

		if (m_Inventory[i])
			itemlist.push_back(m_Inventory[i]->GetScript<ER_DataScript_Item>()->m_eItemCode);
	}

	m_CraftList.clear();

	for (int Litem = 0; Litem < itemlist.size(); ++Litem)
	{
		for (int Ritem = Litem + 1; Ritem < itemlist.size(); ++Ritem)
		{
			int tmp = -1;

			// ���۰����� �������̴�.
			if (S_OK == ER_ItemMgr::GetInst()->SearchRecipe(itemlist[Litem], itemlist[Ritem], tmp))
			{
				// �ش�������� �����ʿ� ��Ͽ� �ִ��� ����
				if (m_IngredientList.end() != m_IngredientList.find(tmp))
					m_CraftList.push_back(tmp);
			}
		}
	}
}

bool ER_DataScript_Character::CraftItem(UINT _Item)
{
	// �ش� �������� ��������� ��� ���� �˻�
	ER_RECIPE recipe = {};
	recipe.recipe = ER_ItemMgr::GetInst()->m_umapIngredient.find(_Item)->second;

	CGameObject** ItemSlot1 = nullptr;
	CGameObject** ItemSlot2 = nullptr;

	for (int i = 0; i < 10; ++i)
	{
		// 2�� �������� �� ã����� Ž�� ����
		if (ItemSlot1 && ItemSlot2)
			break;

		// ���â�� �������� �ִ� ���
		if (i < 5 && m_Equipment[i])
		{
			UINT Itemid = m_Equipment[i]->GetScript<ER_DataScript_Item>()->GetCode();

			if (!ItemSlot1 && Itemid == recipe.ingredient_1)
				ItemSlot1 = &m_Equipment[i];
			else if (!ItemSlot2 && Itemid == recipe.ingredient_2)
				ItemSlot2 = &m_Equipment[i];
		}

		// �κ��丮�� �������� �ִ°��
		if (m_Inventory[i])
		{
			UINT Itemid = m_Inventory[i]->GetScript<ER_DataScript_Item>()->GetCode();

			if (!ItemSlot1 && Itemid == recipe.ingredient_1)
				ItemSlot1 = &m_Inventory[i];
			else if (!ItemSlot2 && Itemid == recipe.ingredient_2)
				ItemSlot2 = &m_Inventory[i];
		}
	}

	// ���� �� ��� ������ ������ delete
	// odelete(*ItemSlot1);
	// (*ItemSlot1) = nullptr;
	// odelete (*ItemSlot2);
	// (*ItemSlot2) = nullptr;

	delete* ItemSlot1;
	(*ItemSlot1) = nullptr;
	delete* ItemSlot2;
	(*ItemSlot2) = nullptr;

	// ������ ������ �Ŵ������� Ŭ������ �����´�.
	CGameObject* NewItem = ER_ItemMgr::GetInst()->GetItemObj(_Item)->Clone();

	int InventoryIdx = -1;
	// ����ִ� �κ��丮�� �������� �ִ´�.
	for (int i = 0; i < 10; ++i)
	{
		if (nullptr == m_Inventory[i])
		{
			m_Inventory[i] = NewItem;
			InventoryIdx = i;
			break;
		}
	}

	// �����۽���Ÿ���� Ȯ���ϰ� �ش� ������ ����ְų� ����� �� ������� SwapItem�� �����Ѵ�.
	ER_DataScript_Item* NewItemInfo = NewItem->GetScript<ER_DataScript_Item>();
	if (!m_Equipment[NewItemInfo->GetSlot()] || m_Equipment[NewItemInfo->GetSlot()]->GetScript<ER_DataScript_Item>()->GetGrade() < NewItemInfo->GetGrade())
	{
		SwapItem(&m_Inventory[InventoryIdx], &m_Equipment[NewItemInfo->GetSlot()]);
	}


	// ������ �������� �ʿ� ���� ��Ͽ� �ִ°�� ī��Ʈ�� ������ 0�̵Ȱ�� �����ش�.
	unordered_map<UINT, int>::iterator iter = m_IngredientList.find(_Item);
	if (iter->second == 1)
		m_IngredientList.erase(iter);
	else
		iter->second--;

	ItemInfoUpdate();

	// �������� �����ߴٸ� ������ �������� ��޿� ���� ����ġ ����
	switch (NewItemInfo->GetGrade())
	{
	case (UINT)ER_ITEM_GRADE::UNCOMMON:
		m_Stats->iExp += 40;
		break;
	case (UINT)ER_ITEM_GRADE::RARE:
		m_Stats->iExp += 60;
		break;
	case (UINT)ER_ITEM_GRADE::EPIC:
		m_Stats->iExp += 80;
		break;
	}

	return true;
}

void ER_DataScript_Character::BeginOverlap(CCollider3D* _Other)
{

}

void ER_DataScript_Character::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::EndOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Character::BeginRayOverlap()
{
	int a = 3;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::OnRayOverlap()
{
}

void ER_DataScript_Character::EndRayOverlap()
{
	int a = 0;
	if (GetOwner()->GetRenderComponent() != nullptr && GetOwner()->GetRenderComponent()->GetMaterial(0) != nullptr)
	{
		GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_3, &a);
	}
}

void ER_DataScript_Character::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strKey, _File);
	SaveWString(m_strName, _File);
	fwrite(&m_STDStats, sizeof(ER_Initial_Stats), 1, _File);
	SaveResRef(m_PortraitTex.Get(), _File);
	SaveResRef(m_FullTax.Get(), _File);
	SaveResRef(m_MapTex.Get(), _File);

	int SkillSize = (int)m_SkillList.size();
	fwrite(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		m_SkillList[i]->Save(_File);
		SaveResRef(m_SkillList[i]->TexSkill.Get(), _File);
	}
}

void ER_DataScript_Character::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strKey, _File);
	LoadWString(m_strName, _File);
	fread(&m_STDStats, sizeof(ER_Initial_Stats), 1, _File);
	LoadResRef(m_PortraitTex, _File);
	LoadResRef(m_FullTax, _File);
	LoadResRef(m_MapTex, _File);

	int SkillSize = (int)m_SkillList.size();
	fread(&SkillSize, sizeof(int), 1, _File);

	for (int i = 0; i < SkillSize; ++i)
	{
		ER_SKILL* Skill = onew(ER_SKILL);
		Skill->Load(_File);
		LoadResRef(Skill->TexSkill, _File);

		m_SkillList.push_back(Skill);
	}
}