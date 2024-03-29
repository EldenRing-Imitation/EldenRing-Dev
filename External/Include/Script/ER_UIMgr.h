#pragma once
#include <Engine\CSingleton.h>

class ER_UIScript_ItemSlot;
class ER_DataScript_ItemBox;

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
    CGameObject* StatusBar_Gauge[2];                        // 0 : HP, 1 : SP
    
    // [ Skill ]
    CGameObject* StatusBar_SkillSlot[4];                    // ��ų��ư / ��Ÿ��
    CGameObject* StatusBar_SkillLevelUpBtn[4];              // ��ư ��ũ��Ʈ
    
    // ==================
    // [ Character Info ]
    // ==================
    CGameObject* StatusBar_CharacterInfo;                   // �ɷ�ġ â + ����
    CGameObject* StatusBar_CharacterInfo_Portrait;          // ĳ���� �ʻ�ȭ
    CGameObject* StatusBar_CharacterInfo_Level;             // ����
    CGameObject* StatusBar_CharacterInfo_Exp;               // ����ġ ��

    CGameObject* StatusBar_CharacterInfo_EquipMent[5];      // ���â

    // =============
    // [ Inventory ]
    // =============
    CGameObject* StatusBar_Inventory;
    CGameObject* StatusBar_Inventory_Slot[10];

    // =============
    // [ CraftList ]
    // =============

    CGameObject*            StatusBar_CraftList[7];

    // ===========
    // [ ItemBox ]
    // ===========

    CGameObject*            ItemBoxUI;

    // ===============
    // [ Cursor Func ]
    // ===============
    CGameObject*            m_pCurDragItem;
    ER_UIScript_ItemSlot*   m_pDragItemSlot;                               // drag n drop temp Obj
    ER_UIScript_ItemSlot*   m_pDropItemSlot;

private:
    void LobbyUI();
    void InGameUI();

    void CreateCenter();
    void CreateCharacterInfo();
    void CreateInventory();
    void CreateItemBoxUI();

public:
    void init();
    void tick();

    void SpawnUI();
    void RegistPlayerCharacetr();

    void OpenItemBoxUI(ER_DataScript_ItemBox* _ItemBox);
    void CloseItemBoxUI();

public:
    // [ Skill Slot Func ]
    void SetToggle(int i);

public:
    void RegistDragItemSlot(ER_UIScript_ItemSlot* _SrcSlot);
    void RegistDropItemSlot(ER_UIScript_ItemSlot* _DestSlot);
    void SwapItem();    // UIMgr�� Drag , Drop ���ʿ� obj�� ������ ����, ������ �Ѵ� null
};

