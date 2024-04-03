#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Character :
    public CScript
{
private:
    // [ Info ]
    wstring                     m_strKey;
    wstring                     m_strName;              // ����ü �̸�
    ER_Initial_Stats            m_STDStats;             // ����ü �ʱ� �ɷ�ġ

    Ptr<CTexture>               m_PortraitTex;          // �ʻ�ȭ �ؽ���
    Ptr<CTexture>               m_FullTax;              // ���� �Ϸ�
    Ptr<CTexture>               m_MapTex;               // �̴ϸ� �ؽ���
    Ptr<CSound>                 m_LevelUpSound;         // ������ ����
    vector<ER_SKILL*>           m_SkillList;            // ���� ��ų             

    // [ Stats ]
    tIngame_Stats*              m_Stats;                // ���� �ɷ�ġ

    // [ Skill ]
    UINT                        m_SkillPoint;           // ��ų ������ ���� ���� ����Ʈ
    float                       m_fSPRegenTime;         // SPRegen Tiem

    // [ Status Effect ]
    tStatus_Effect*             m_StatusEffect;         // ����ȿ�� ����ü
    bool                        m_bGameDead;            // ĳ���� ����Ǵ�
    bool                        m_bOutofContorl;        // ����Ұ�����

    // [ Item ]
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // ���â
    CGameObject*                m_Inventory[10];                        // �κ��丮
    UINT                        m_RootItem[5];                          // ���� ��ǥ ������
    vector<UINT>                m_CraftList;                            // ���۰��� ������ ���

    // [ Debug / CoolDown Delete ]
    bool                        DebugMode;
    unordered_map<UINT, int>    m_IngredientList;                       // ��� ������ ���
    unordered_map<UINT, int>    m_NeedFarmingItems;                     // �ʿ� �Ĺ� ������ ���

    mutex                       m_mMutex;

    // [ Developer cheat ]
    bool                        bCoolDownCheat;
    bool                        bInvincibleCheat;

public:
    // [���� ����]
    void                        StatusUpdate();                            // Status ���� �Լ� : ������ , �����ۺ���, ����/����
    void                        HPRegen(float _magnification = 1.f);      // �ڿ�ü��ȸ��. �⺻ 1����
    void                        SPRegen(float _magnification = 1.f);      // �ڿ����׹̳�ȸ��. �⺻ 1����

    void                        LevelUP();                                 // ������ �� �����ϴ� �Լ� ����

public:
    void         init();
    virtual void begin() override;
    virtual void tick() override;

    const wstring&      GetCharacterName() { return m_strKey; }
    const wstring&      GetCharacterKorName() { return m_strName; }

public:
    // [Status]
    ER_Ingame_Stats*    GetStatus() { return m_Stats; }
    UINT*               GetSkillPoint() { return &m_SkillPoint; }

    Ptr<CTexture>       GetPortraitTex() { return m_PortraitTex; }

    tStatus_Effect*     GetStatusEffect() { return m_StatusEffect; }
    void                SetGameDead() { m_bGameDead = true; }
    const bool          IsDeadState() { return m_bGameDead; }
    const bool          IsOutofControl() { return m_bOutofContorl; }

    // [Skill]
    vector<ER_SKILL*>&  GetSkillList() { return m_SkillList; }
    ER_SKILL*           GetSkill(int _Idx) { return m_SkillList[_Idx]; }
    ER_SKILL**          GetSkillAdress(int _Idx) { return &m_SkillList[_Idx]; }

    // [Item]
    CGameObject**       GetAllEquipItem() { return m_Equipment; }
    CGameObject**       GetEquipItem(UINT _SlotType) { return &m_Equipment[_SlotType]; }
    vector<UINT>*       GetCraftListAdress() { return &m_CraftList; }

    CGameObject**       GetAllInvenItem() { return m_Inventory; }
    CGameObject*        GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject*        GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    unordered_map<UINT, int> GetIngredientList() { return m_IngredientList; }
    unordered_map<UINT, int> GetNeedFarmingList() { return m_NeedFarmingItems; }

    void                SetRootItem(UINT* _RootItem, int _cnt)
    {
        for (int i = 0; i < _cnt; ++i)
        {
            m_RootItem[i] = _RootItem[i];
        }
    }
    UINT*               GetRootItem() { return m_RootItem; }

    bool                SwapItem(CGameObject** _DragItem, CGameObject** _DropItem);
    void                AcquireItem(CGameObject** _BoxSlot);  // ����������
    void                ItemInfoUpdate();
    bool                CraftItem(UINT _Item);    // ����, ��ώ��,��ώ��

public:
    virtual void        BeginOverlap(CCollider3D* _Other) override;
    virtual void        OnOverlap(CCollider3D* _Other) override;
    virtual void        EndOverlap(CCollider3D* _Other) override;

    virtual void        BeginRayOverlap() override;
    virtual void        OnRayOverlap() override;
    virtual void        EndRayOverlap() override;

public:
    virtual void        SaveToLevelFile(FILE* _File) override;
    virtual void        LoadFromLevelFile(FILE* _FILE) override;

public:
    ER_DataScript_Character();
    ER_DataScript_Character(const ER_DataScript_Character& _origin);
    ~ER_DataScript_Character();
    
    CLONE(ER_DataScript_Character);


    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};

