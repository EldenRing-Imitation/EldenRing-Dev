#include "pch.h"
#include "ER_CharacterMgr.h"

#include "ER_PlayerScript.h"
#include "ER_GameSystem.h"

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
}

CGameObject* ER_CharacterMgr::GetCharacter(const wstring& _key)
{
    CGameObject* Character = new CGameObject(*m_mapCharacters.find(_key)->second);
    Character->AddComponent(new ER_PlayerScript);
    Character->AddComponent(new CFindPath);

    Character->Transform()->SetRelativeScale(Vec3(1.1f, 1.1f, 1.1f));

    //���ӽý��ۿ� �÷��̾��� ĳ���͸� �Ҵ�.
    ER_GameSystem::GetInst()->GetInst()->SetPlayerCharacter(Character);

    return Character;
}
