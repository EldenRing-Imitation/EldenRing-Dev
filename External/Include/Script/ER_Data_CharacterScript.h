#pragma once
#include <Engine\CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_Data_CharacterScript :
    public CScript
{
private:
    wstring             m_strKey;
    wstring             m_strName;              // ����ü �̸�
    ER_STD_CHAR_STATS   m_STDStats;             // ����ü �ʱ� �ɷ�ġ

    Ptr<CTexture>       m_PortraitTex;          // �ʻ�ȭ �ؽ���
    Ptr<CTexture>       m_FullTax;              // ���� �Ϸ�
    Ptr<CTexture>       m_MapTex;               // �̴ϸ� �ؽ���

    // ����ü ���� �ɷ�ġ (�ʱ�ɷ�ġ * ����, + ������, + ��ųȿ��)


    // [�κ��丮 15��]
    // ���� 5��
    // �κ��丮���� 10��

    // [�Լ�]
    // �ɷ�ġ ���� �Լ� (������ ����, ������, �����̻� �� ��ȣ�ۿ�� �������� ȣ��)

    CLONE(ER_Data_CharacterScript);
public:
    void init();
    virtual void begin() override;
    virtual void tick() override;


public:
    ER_Data_CharacterScript();
    ~ER_Data_CharacterScript();

    friend class ER_CharacterMgr;
    friend class CharacterDataUI;
};

