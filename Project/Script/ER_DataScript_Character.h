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

    // �����Ǵ�
    tStatus_Effect*             m_StatusEffect;         // ����ȿ�� ����ü
    bool                        m_bGameDead;            // ĳ���� ����Ǵ�
    bool                        m_bOutofContorl;        // ����Ұ�����


    // ���â
    CGameObject*                m_Equipment[(UINT)ER_ITEM_SLOT::END];   // ���ĭ
    CGameObject*                m_Inventory[10];                        // �κ��丮 10ĭ

    CGameObject*                m_aStatBar[4];          //[0] : HPBAR, [1] : ReturnBar, [2] : SteminarBar, [3] : LevelText
    float                       m_aStatPosOffset[4];    //���ȵ��� x ��ġ �����µ�.
    bool                        m_bHPChangeTrigger;     //HP ����� Ʈ����

public:
    // [���� ����]
    void StatusUpdate();    // Status ���� �Լ� : ������ , �����ۺ���, ����/����
    // ��ų ��Ÿ�� ���� �Լ�


public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

    const wstring& GetCharacterName() { return m_strKey; }

public:
    // [Status]
    ER_Ingame_Stats* GetStatus() { return m_Stats; }

    Ptr<CTexture> GetPortraitTex() { return m_PortraitTex; }
    
    tStatus_Effect* GetStatusEffect() { return m_StatusEffect; }
    const bool IsDeadState() { return m_bGameDead; }
    const bool IsOutofControl() { return m_bOutofContorl; }

    // [Skill]
    vector<ER_SKILL*>& GetSkillList() { return m_SkillList; }
    ER_SKILL* GetSkill(int _Idx) { return m_SkillList[_Idx]; }

    // [Item]
    CGameObject** GetAllEquipItem() { return m_Equipment; }
    CGameObject* GetEquipItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject** GetAllInvenItem() { return m_Inventory; }
    CGameObject* GetInvenItem(UINT _SlotX, UINT _SlotY) { return m_Equipment[_SlotX * 5 + _SlotY]; }
    CGameObject* GetInvenItem(UINT _SlotType) { return m_Equipment[_SlotType]; }

    CGameObject* ItemAcquisition(CGameObject* _ItemObj);

    bool SwapItem(CGameObject* _DragmItem, CGameObject* _DropItem);

public:
    void CreateStatBar();
    void UpdateStatBar(); // �� ƽ ������Ʈ
    void ChangeStatBar(); // ��ȯ�� �ʿ��� ���� ȣ��
    void ChangeHPReturnBar(); //HPReturnBar ����� ȣ��

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void BeginRayOverlap() override;
    virtual void OnRayOverlap() override;
    virtual void EndRayOverlap() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

public:
    ER_DataScript_Character();
    ER_DataScript_Character(const ER_DataScript_Character& _origin);
    ~ER_DataScript_Character();
    
    CLONE(ER_DataScript_Character);

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};

