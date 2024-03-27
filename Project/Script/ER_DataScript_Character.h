#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Character :
    public CScript
{
private:
    wstring                     m_strKey;
    wstring                     m_strName;              // ����ü �̸�
    ER_Initial_Stats            m_STDStats;             // ����ü �ʱ� �ɷ�ġ

    Ptr<CTexture>               m_PortraitTex;          // �ʻ�ȭ �ؽ���
    Ptr<CTexture>               m_FullTax;              // ���� �Ϸ�
    Ptr<CTexture>               m_MapTex;               // �̴ϸ� �ؽ���
    vector<ER_SKILL*>           m_SkillList;            // ���� ��ų             

    // ����ü ���� �ɷ�ġ (�ʱ�ɷ�ġ * ����, + ������, + ��ųȿ��)
    tIngame_Stats*              m_Stats;                // ���� �ɷ�ġ
    ER_SKILL*                   m_Skill[4];             // ������뽺ų

    UINT                        m_SkillPoint;           // ��ų ������ ���� ���� ����Ʈ
    float                       m_fSPRegenTime;         // SPRegen Tiem

    // �����Ǵ�
    tStatus_Effect*             m_StatusEffect;         // ����ȿ�� ����ü
    bool                        m_bGameDead;            // ĳ���� ����Ǵ�
    bool                        m_bOutofContorl;        // ����Ұ�����


    // ���â
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // ���ĭ
    // �κ��丮
    CGameObject*                m_Inventory[10];                        // �κ��丮 10ĭ

public:
    // [���� ����]
    void                        StatusUpdate();                            // Status ���� �Լ� : ������ , �����ۺ���, ����/����
    void                        HPRegen(float _magnification = 1.f);      // �ڿ�ü��ȸ��. �⺻ 1����
    void                        SPRegen(float _magnification = 1.f);      // �ڿ����׹̳�ȸ��. �⺻ 1����

    void                        LevelUP();                                 // ������ �� �����ϴ� �Լ� ����

public:
    void init();
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
    ER_SKILL*           GetSkill(int _Idx) { return m_Skill[_Idx]; }
    ER_SKILL**          GetSkillAdress(int _Idx) { return &m_Skill[_Idx]; }
    
    void                ChangeSkill(int _Idx);
    void                SkillSlotInit();

    // [Item]
    CGameObject**       GetAllEquipItem() { return m_Equipment; }
    CGameObject*        GetEquipItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject**       GetAllInvenItem() { return m_Inventory; }
    CGameObject*        GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject*        GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject*        ItemAcquisition(CGameObject* _ItemObj);

    bool                SwapItem(CGameObject** _DragItem, CGameObject** _DropItem);
    void                AcquireItem(CGameObject** _BoxSlot);  // ����������
    void                CraftItem(CGameObject** _iSlot1, CGameObject** _iSlot2);    // ����, ��ώ��,��ώ��

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

