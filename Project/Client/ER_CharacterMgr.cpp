#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ImGuiMgr.h"
#include "CharacterDataUI.h"

ER_CharacterMgr::ER_CharacterMgr()
{
}

ER_CharacterMgr::~ER_CharacterMgr()
{
    Safe_Del_Map(m_mapCharacters);
}

void ER_CharacterMgr::init()
{
    // ĳ���� ������ �ε�

    ((CharacterDataUI*)ImGuiMgr::GetInst()->FindUI("##CharacterDataUI"))->RegistCharacters();
}

void ER_CharacterMgr::tick()
{
}

int ER_CharacterMgr::SpawnCharacter(const wstring& _key)
{
    return 0;
}
