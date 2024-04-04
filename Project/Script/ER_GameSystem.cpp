#include "pch.h"
#include "ER_GameSystem.h"

#include "ER_BattleSystem.h"

#include "ER_UIMgr.h"
#include "ER_ItemMgr.h"
#include "ER_CharacterMgr.h"

#include "ER_Cursor.h"
#include <Engine\ptr.h>
#include <Engine\CResMgr.h>
#include <Engine\CSound.h>

ER_GameSystem::ER_GameSystem()
	: m_pCursor(nullptr)
	, m_fGameTime(0.f)
	, m_bGaming(false)
	, m_BGMSound(nullptr)
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

	m_BGMSound->Play(0, 0.2f, false);
}

void ER_GameSystem::init()
{
	// ���ҽ� �ε� : ImGui ��� ���ϴ� ��� �Ʒ� �Լ��� ���ҽ� �ε�
	AllResourceLoad();

	ER_ItemMgr::GetInst()->init();
	ER_CharacterMgr::GetInst()->init();
	ER_BattleSystem::GetInst()->init();
	ER_UIMgr::GetInst()->init();
	
	m_pCursor = onew(CGameObject);
	ER_Cursor* csrScript = onew(ER_Cursor);
	m_pCursor->AddComponent(csrScript);
	csrScript->init();
	
	m_BGMSound = CResMgr::GetInst()->FindRes<CSound>(L"BSER_AreaBGM_UPTOWN.wav");
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