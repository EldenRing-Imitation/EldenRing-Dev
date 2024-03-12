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
    // 캐릭터 데이터 로딩
    Load();

    ((CharacterDataUI*)ImGuiMgr::GetInst()->FindUI("##CharacterDataUI"))->RegistCharacters();
}

CGameObject* ER_CharacterMgr::GetCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);

    return Character;
}
