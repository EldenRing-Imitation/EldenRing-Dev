#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_BattleSystem.h"

#include "ER_UIMgr.h"
#include "ER_ItemMgr.h"
#include "ER_CharacterMgr.h"

#include "ER_Cursor.h"

ER_GameSystem::ER_GameSystem()
	: m_pCursor(nullptr)
	, m_fGameTime(0.f)
	, m_bGaming(false)
{
}

ER_GameSystem::~ER_GameSystem()
{
	m_pCharacters.clear();
}


ER_Cursor* ER_GameSystem::GetCursor()
{
	return m_pCursor->GetScript<ER_Cursor>();
}

void ER_GameSystem::GameStart()
{
	// ���ӽ��� �ð� 
	m_bGaming = true;

	// UI ����
	ER_UIMgr::GetInst()->SpawnUI();

	// Ŀ�� �߰�
	SpawnGameObject(m_pCursor, L"UI");

	// ������ ����
	ER_ItemMgr::GetInst()->SpawnItemLevel();
}

void ER_GameSystem::init()
{
	// ���ҽ� �ε� : ImGui ��� ���ϴ� ��� �Ʒ� �Լ��� ���ҽ� �ε�
	// AllResourceLoad();

	ER_ItemMgr::GetInst()->init();
	ER_CharacterMgr::GetInst()->init();
	ER_BattleSystem::GetInst()->init();
	ER_UIMgr::GetInst()->init();
	
	m_pCursor = new CGameObject;
	m_pCursor->AddComponent(new ER_Cursor);
	m_pCursor->GetScript<ER_Cursor>()->init();
}

void ER_GameSystem::progress()
{
	tick();
}

void ER_GameSystem::tick()
{
	ER_UIMgr::GetInst()->tick();

	if (m_bGaming)
		m_fGameTime += DT;
}