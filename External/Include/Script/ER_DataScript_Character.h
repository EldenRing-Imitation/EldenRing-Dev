#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Character :
    public CScript
{
private:
    wstring             m_strKey;
    wstring             m_strName;              // ����ü �̸�
    ER_Initial_Stats    m_STDStats;             // ����ü �ʱ� �ɷ�ġ

    Ptr<CTexture>       m_PortraitTex;          // �ʻ�ȭ �ؽ���
    Ptr<CTexture>       m_FullTax;              // ���� �Ϸ�
    Ptr<CTexture>       m_MapTex;               // �̴ϸ� �ؽ���

    // ����ü ���� �ɷ�ġ (�ʱ�ɷ�ġ * ����, + ������, + ��ųȿ��)
    ER_Ingame_Stats     m_Stats;                // ���� �ɷ�ġ

    // [�κ��丮 15��]
    // ���� 5��
    // �κ��丮���� 10��

    // [�Լ�]
    // �ɷ�ġ ���� �Լ� (������ ����, ������, �����̻� �� ��ȣ�ۿ�� �������� ȣ��)
    void StatusUpdate();    // Status ���� �Լ�

    CLONE(ER_DataScript_Character);
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    const ER_Ingame_Stats& GetStatus() { return m_Stats; }
    Ptr<CTexture> GetPortraitTex() { return m_PortraitTex; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

public:
    ER_DataScript_Character();
    ER_DataScript_Character(const ER_DataScript_Character& _origin);
    ~ER_DataScript_Character();

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};

