#pragma once
#include <Engine\CScript.h>


class ER_Data_CharacterScript :
    public CScript
{
private:
    // [����]
    wstring m_strName;  // ����ü �̸�
    // ����ü �ʱ� �ɷ�ġ

    // �ʻ�ȭ �ؽ���
    // ���� �Ϸ�

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
};

