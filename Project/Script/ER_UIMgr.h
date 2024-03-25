#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr :
    public CSingleton<ER_UIMgr>
{
    SINGLE(ER_UIMgr);
private:
    // =====================
    // [ Status Bar Center ]
    // =====================
    CGameObject* StatusBar_Center;              
    
    // [ HP / SP ]
    CGameObject* StatusBar_Gauge[2];             // 0 : HP, 1 : SP
    
    // [ Skill ]
    CGameObject* StatusBar_SkillSlot[4];        // ��ų��ư / ��Ÿ��
    CGameObject* StatusBar_SkillLevelUpBtn[4];  // ��ư ��ũ��Ʈ
    
    // ==================
    // [ Character Info ]
    // ==================
    CGameObject* StatusBar_CharacterInfo;                   // �ɷ�ġ â + ����
    CGameObject* StatusBar_CharacterInfo_Portrait;          // ĳ���� �ʻ�ȭ
    CGameObject* StatusBar_CharacterInfo_Level;             // ����
    CGameObject* StatusBar_CharacterInfo_Exp;               // ����ġ ��

    CGameObject* StatusBar_CharacterInfo_EquipMent[5];      // ���â

    // =====================
    // [ Inventory ]
    // =====================
    CGameObject* StatusBar_Inventory;
    CGameObject* StatusBar_Inventory_Slot[10];


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

