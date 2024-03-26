#pragma once
#include <Engine\CScript.h>

class ER_DataScript_Item;

class ER_UIScript_ItemSlot :
    public CScript
{
    enum class eSlotType
    {
        COMMON,
        EQUIPMENT,
    };

private:
    CGameObject** m_Slot; 
    ER_DataScript_Item* m_ItemData;
    // Box�� ����ִ� �������� �״�� �޾Ƽ� �����ֱ⸸.

    eSlotType m_SlotType;

    int m_PrevItemID;
    int m_CurItemID;

public:
    void            init();
    virtual void    begin() override;
    virtual void    tick() override;
    
    void RegistSlotAdress(CGameObject** _ItemSlotAddress, eSlotType _SlotType);

    bool IsItemInSlot();

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

public:
    ER_UIScript_ItemSlot();
    ~ER_UIScript_ItemSlot();

    CLONE(ER_UIScript_ItemSlot);

    friend class ER_UIMgr;
    friend class ER_UIScript_ItemBox;
};

