#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr :
    public CSingleton<ER_UIMgr>
{
    SINGLE(ER_UIMgr);
private:
    // [ Status Bar ]
    CGameObject* StatusBar_Center;              // ��ų��ư��ũ��Ʈ / ��Ÿ��
    CGameObject* StatusBar_CharacterInfo;       // ���â ��ư��ũ��Ʈ
    CGameObject* StatusBar_Inventory;           // ��ư ��ũ��Ʈ

    // [ HP / SP ]
    CGameObject* StatusBar_Gauge[2];             // 0 : HP, 1 : SP

    // [ Skill ]
    CGameObject* StatusBar_SkillSlot[4];        // ��ų��ư
    CGameObject* StatusBar_SkillLevelUpBtn[4];  // ��ư ��ũ��Ʈ

private:
    void LobbyUI();
    void InGameUI();

    void CreateCenter();
    void CreateCharacterInfo();
    void CreateInventory();

public:
    void init();
    void tick();

    void SpawnUI();

    void RegistPlayerCharacetr();
};

