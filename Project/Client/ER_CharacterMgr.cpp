#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ImGuiMgr.h"
#include "CharacterDataUI.h"

#include <Script\ER_PlayerScript.h>

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
    Load();

    ((CharacterDataUI*)ImGuiMgr::GetInst()->FindUI("##CharacterDataUI"))->RegistCharacters();
}

CGameObject* ER_CharacterMgr::GetCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    Character->AddComponent(new ER_PlayerScript);
    Character->AddComponent(new CFindPath);

    Character->Transform()->SetRelativeScale(Vec3(1.3f, 1.3f, 1.3f));

    return Character;
}
