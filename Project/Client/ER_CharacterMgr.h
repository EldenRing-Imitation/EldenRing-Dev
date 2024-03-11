#pragma once
#include <Engine/CSingleton.h>
#include <Engine/CGameObject.h>

class ER_CharacterMgr :
    public CSingleton<ER_CharacterMgr>
{
    SINGLE(ER_CharacterMgr);

private:
    map<wstring, CGameObject*> m_mapCharacters;  // ĳ���� ������ƮǮ

private:
    int Save();
    int Load();

public:
    void init();
    void tick();

    int SpawnCharacter(const wstring& _key);

    friend class CharacterDataUI;
};