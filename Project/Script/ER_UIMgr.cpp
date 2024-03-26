#include "pch.h"
#include "ER_UIMgr.h"

#include <Engine\components.h>
#include <Engine\CResMgr.h>
#include <Engine\CText.h>

#include "ER_GameSystem.h"
#include "ER_DataScript_Character.h"
#include "ER_UIScript_SkillSlot.h"
#include "ER_UIScript_SkillUpBtn.h"
#include "ER_UIScript_StatusBarGauge.h""

#include "ER_UIScript_ItemSlot.h"
#include "ER_DataScript_Item.h"

#include "ER_UIScript_ItemBox.h"
#include "ER_DataScript_ItemBox.h"

ER_UIMgr::ER_UIMgr()
	: StatusBar_Center(nullptr)
	, StatusBar_CharacterInfo(nullptr)
	, StatusBar_Inventory(nullptr)
	, StatusBar_SkillSlot{}
	, StatusBar_SkillLevelUpBtn{}
	, StatusBar_Gauge{}
	, ItemBoxUI(nullptr)
	, m_pCurDragItem(nullptr)
	, m_pDragItemSlot(nullptr)
	, m_pDropItemSlot(nullptr)
	, StatusBar_CharacterInfo_EquipMent{}
	, StatusBar_CharacterInfo_Portrait(nullptr)
{
}

ER_UIMgr::~ER_UIMgr()
{
}

void ER_UIMgr::LobbyUI()
{
}

void ER_UIMgr::InGameUI()
{
	// [ ���� ������Ʈ ]
	// 1. �߾Ӻ���
	// 2. ��ų ������ ��ư 4��
}

void ER_UIMgr::CreateCenter()
{
	// [ �߾� UI ���� ���� ]
	StatusBar_Center = new CGameObject;
	StatusBar_Center->SetName(L"UI_StatusBar_Center");
	AddComponents(StatusBar_Center, _TRANSFORM | _MESHRENDER | _UICOMPONENT);
	
	StatusBar_Center->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_Center->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_Center->MeshRender()->GetDynamicMaterial(0);

	StatusBar_Center->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBgCenter.png"));
	StatusBar_Center->Transform()->LoadPrefab(L"UI_StatusBar_Center");

	for (int i = 0; i < 4; ++i)
	{
		// [ ��ų������ �ܺ� ��ų���� ���� ���� ]
		StatusBar_SkillLevelUpBtn[i] = new CGameObject;
		StatusBar_SkillLevelUpBtn[i]->AddComponent(new ER_UIScript_SkillUpBtn);
		StatusBar_SkillLevelUpBtn[i]->GetScript<ER_UIScript_SkillUpBtn>()->SetUI(i);
		
		// [ �߾Ӻ��� ���� ��ų���� UI ���� ]
		StatusBar_SkillSlot[i] = new CGameObject;
		StatusBar_SkillSlot[i]->AddComponent(new ER_UIScript_SkillSlot);
		StatusBar_SkillSlot[i]->GetScript<ER_UIScript_SkillSlot>()->SetUI(i, StatusBar_SkillLevelUpBtn[i]);
		
		// ��ũ��Ʈ �߰����ְ� ���ڷ� �����Լ� �־��ֱ�
		StatusBar_Center->AddChild(StatusBar_SkillSlot[i]);
	}

	// [ �������ͽ��� ������ UI ���� ]
	for (int i = 0; i < 2; ++i)
	{
		StatusBar_Gauge[i] = new CGameObject;
		StatusBar_Gauge[i]->AddComponent(new ER_UIScript_StatusBarGauge);
		StatusBar_Gauge[i]->GetScript<ER_UIScript_StatusBarGauge>()->SetUI(i);

		StatusBar_Center->AddChild(StatusBar_Gauge[i]);
	}

}

void ER_UIMgr::CreateCharacterInfo()
{
	// [ ĳ�������� UI ���� ���� ]
	StatusBar_CharacterInfo = new CGameObject;
	StatusBar_CharacterInfo->SetName(L"UI_StatusBar_CharacterInfo");
	AddComponents(StatusBar_CharacterInfo, _TRANSFORM | _MESHRENDER | _TEXT | _UICOMPONENT);

	StatusBar_CharacterInfo->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_CharacterInfo->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_CharacterInfo->MeshRender()->GetDynamicMaterial(0);

	StatusBar_CharacterInfo->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"StatusBar_CharacterInfo.png"));
	StatusBar_CharacterInfo->LoadAllPrefabFromObjName();

	StatusBar_CharacterInfo_Portrait = new CGameObject;
	StatusBar_CharacterInfo_Portrait->SetName(L"UI_StatusBar_CharacterInfo_Portrait");
	AddComponents(StatusBar_CharacterInfo_Portrait, _TRANSFORM | _MESHRENDER);
	StatusBar_CharacterInfo_Portrait->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_CharacterInfo_Portrait->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_CharacterInfo_Portrait->MeshRender()->GetDynamicMaterial(0);
	StatusBar_CharacterInfo_Portrait->LoadAllPrefabFromObjName();
	
	StatusBar_CharacterInfo_Level = new CGameObject;
	StatusBar_CharacterInfo_Level->SetName(L"UI_StatusBar_CharacterInfo_Level");
	AddComponents(StatusBar_CharacterInfo_Level, _TRANSFORM | _MESHRENDER | _TEXT);
	StatusBar_CharacterInfo_Level->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_CharacterInfo_Level->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_CharacterInfo_Level->MeshRender()->GetDynamicMaterial(0);
	StatusBar_CharacterInfo_Level->LoadAllPrefabFromObjName();
	StatusBar_CharacterInfo_Level->Text()->SetCenter(0);

	StatusBar_CharacterInfo_Level->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_Map_PointPin_01.png"));
	
	const wchar_t* EquipTypeTexKey[5] = { L"Ico_Status_Weapon.png", L"Ico_Status_Armor.png", L"Ico_Status_Head.png",	L"Ico_Status_Arm.png", L"Ico_Status_Leg.png" };

	for (int i = 0; i < 5; ++i)
	{
		wstring name = L"StatusBar_CharacterInfo_EquipMent" + std::to_wstring(i);
		StatusBar_CharacterInfo_EquipMent[i] = new CGameObject;
		StatusBar_CharacterInfo_EquipMent[i]->SetName(name);
		StatusBar_CharacterInfo_EquipMent[i]->AddComponent(new ER_UIScript_ItemSlot);
		StatusBar_CharacterInfo_EquipMent[i]->GetScript<ER_UIScript_ItemSlot>()->init();
		StatusBar_CharacterInfo->AddChild(StatusBar_CharacterInfo_EquipMent[i]);
		StatusBar_CharacterInfo_EquipMent[i]->LoadAllPrefabFromObjName();
		StatusBar_CharacterInfo_EquipMent[i]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_6, CResMgr::GetInst()->FindRes<CTexture>(EquipTypeTexKey[i]));
	}

	StatusBar_CharacterInfo->AddChild(StatusBar_CharacterInfo_Portrait);
	StatusBar_CharacterInfo->AddChild(StatusBar_CharacterInfo_Level);
}

void ER_UIMgr::CreateInventory()
{
	// [ �κ��丮 UI ���� ���� ]
	StatusBar_Inventory = new CGameObject;
	StatusBar_Inventory->SetName(L"UI_StatusBar_Inventory");
	AddComponents(StatusBar_Inventory, _TRANSFORM | _MESHRENDER | _UICOMPONENT);

	StatusBar_Inventory->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	StatusBar_Inventory->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	StatusBar_Inventory->MeshRender()->GetDynamicMaterial(0);

	StatusBar_Inventory->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Img_MainBg_04.png"));
	StatusBar_Inventory->LoadAllPrefabFromObjName();

	for (int i = 0; i < 10; ++i)
	{
		wstring name = L"StatusBar_Inventory_Slot_" + std::to_wstring(i);
		StatusBar_Inventory_Slot[i] = new CGameObject;
		StatusBar_Inventory_Slot[i]->SetName(name);
		StatusBar_Inventory_Slot[i]->AddComponent(new ER_UIScript_ItemSlot);
		StatusBar_Inventory_Slot[i]->GetScript<ER_UIScript_ItemSlot>()->init();
		StatusBar_Inventory->AddChild(StatusBar_Inventory_Slot[i]);
		StatusBar_Inventory_Slot[i]->LoadAllPrefabFromObjName();
	}
}

void ER_UIMgr::CreateItemBoxUI()
{
	ItemBoxUI = new CGameObject;
	ItemBoxUI->AddComponent(new ER_UIScript_ItemBox);
	ItemBoxUI->GetScript<ER_UIScript_ItemBox>()->init();
}

void ER_UIMgr::init()
{
	// �������ͽ� UI ����
	CreateCenter();
	CreateCharacterInfo();
	CreateInventory();
	CreateItemBoxUI();
}

void ER_UIMgr::tick()
{
}

void ER_UIMgr::SpawnUI()
{
	// [ �߾� �������ͽ��� ���� ]
	SpawnGameObject(StatusBar_Center, L"UI");
	SpawnGameObject(StatusBar_CharacterInfo, L"UI");
	SpawnGameObject(StatusBar_Inventory, L"UI");
	SpawnGameObject(ItemBoxUI, L"UI");
	
	ItemBoxUI->SetEnable(false);

	// ��ų ������ ��ư
	for (auto Btn : StatusBar_SkillLevelUpBtn)
		SpawnGameObject(Btn, L"UI");
}

void ER_UIMgr::RegistPlayerCharacetr()
{
	// =====================
	// [ Status Bar Center ]
	// =====================
	// [ SkillSlot ]
	for (int i = 0; i < 4; ++i)
	{
		StatusBar_SkillSlot[i]->GetScript<ER_UIScript_SkillSlot>()->RegistPlayerCharacetr();
	}

	// [ HP / SP Guage ]
	for (int i = 0; i < 2; ++i)
	{
		StatusBar_Gauge[i]->GetScript<ER_UIScript_StatusBarGauge>()->RegistPlayerCharacetr();
	}


	// ===================
	// [ Character Info  ]
	// ===================
	// [ Stats ]
	
	ER_DataScript_Character* PlayerCharacter = ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>();
	StatusBar_CharacterInfo_Portrait->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, PlayerCharacter->GetPortraitTex());
	StatusBar_CharacterInfo_Level->Text()->SetReference(0, 1, (DWORD_PTR)&PlayerCharacter->GetStatus()->iLevel);

	StatusBar_CharacterInfo->Text()->SetReference(0, (UINT)CText::eTextRefType::INT, (DWORD_PTR)&PlayerCharacter->GetStatus()->iAttackPower);
	StatusBar_CharacterInfo->Text()->SetReference(1, (UINT)CText::eTextRefType::INT, (DWORD_PTR)&PlayerCharacter->GetStatus()->iDefense);
	StatusBar_CharacterInfo->Text()->SetReference(2, (UINT)CText::eTextRefType::FLOAT_DP1, (DWORD_PTR)&PlayerCharacter->GetStatus()->fAttackSpeed);
	StatusBar_CharacterInfo->Text()->SetReference(3, (UINT)CText::eTextRefType::FLOAT_DP1, (DWORD_PTR)&PlayerCharacter->GetStatus()->fMovementSpeed);
	StatusBar_CharacterInfo->Text()->SetReference(4, (UINT)CText::eTextRefType::PERCENT, (DWORD_PTR)&PlayerCharacter->GetStatus()->fCriticalStrikeDamage);
	StatusBar_CharacterInfo->Text()->SetReference(5, (UINT)CText::eTextRefType::PERCENT, (DWORD_PTR)&PlayerCharacter->GetStatus()->fCriticalStrikeChance);
	StatusBar_CharacterInfo->Text()->SetReference(6, (UINT)CText::eTextRefType::FLOAT_DP1, (DWORD_PTR)&PlayerCharacter->GetStatus()->fHPRegen);
	StatusBar_CharacterInfo->Text()->SetReference(7, (UINT)CText::eTextRefType::FLOAT_DP1, (DWORD_PTR)&PlayerCharacter->GetStatus()->fSPRegen);
	
	// [ EquipMent ]
	for (int i = 0; i < 5; ++i)
	{
		StatusBar_CharacterInfo_EquipMent[i]->GetScript<ER_UIScript_ItemSlot>()->RegistSlotAdress(&PlayerCharacter->GetAllEquipItem()[i],ER_UIScript_ItemSlot::eSlotType::EQUIPMENT);
	}


	// =============
	// [ Inventory ]
	// =============
	for (int i = 0; i < 10; ++i)
	{
		StatusBar_Inventory_Slot[i]->GetScript<ER_UIScript_ItemSlot>()->RegistSlotAdress(&PlayerCharacter->GetAllInvenItem()[i], ER_UIScript_ItemSlot::eSlotType::COMMON);
	}

}

void ER_UIMgr::OpenItemBoxUI(ER_DataScript_ItemBox* _ItemBox)
{
	ItemBoxUI->GetScript<ER_UIScript_ItemBox>()->RegistItemBoxData(_ItemBox);
	ItemBoxUI->SetEnable(true);
	CResMgr::GetInst()->FindRes<CSound>(L"Locker_01.wav")->Play(1, 0.5f, false);
}

void ER_UIMgr::CloseItemBoxUI()
{
	ItemBoxUI->SetEnable(false);
}

void ER_UIMgr::RegistDragItemSlot(ER_UIScript_ItemSlot* _SrcSlot)
{
	// Slot�� ����Ű�°��� nullptr��� �۾��� �������� �ʴ´�.
	if ((*_SrcSlot->m_Slot))
	{
		m_pDragItemSlot = _SrcSlot;
	}
}

void ER_UIMgr::RegistDropItemSlot(ER_UIScript_ItemSlot* _DestSlot)
{
	// DragItemSlot�� nullptr�̶�� �۾��� �������� �ʴ´�.
	if (m_pDragItemSlot)
	{
		m_pDropItemSlot = _DestSlot;
		
		// SwapItem
		SwapItem();
	}

	// ���� DragITem, DropITem �ʱ�ȭ
	m_pDragItemSlot = nullptr;
	m_pDropItemSlot = nullptr;
}

void ER_UIMgr::SwapItem()
{
	CGameObject** DragItem = &(*m_pDragItemSlot->m_Slot);
	CGameObject** DropItem = &(*m_pDropItemSlot->m_Slot);

	ER_GameSystem::GetInst()->GetPlayerCharacter()->GetScript<ER_DataScript_Character>()->SwapItem(DragItem, DropItem);
}
