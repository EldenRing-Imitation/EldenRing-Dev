#pragma once
#include <Engine\CGameObject.h>

class ER_Character :
    public CGameObject
{
private:
    // �����ͷ� �����س��� ��.
    wstring m_strCharacterKey;  // Character Key
    wstring m_strCharacterName; // Character Name Kr

    // �޽õ�����
    // =========[�Ӽ���]========
    ER_Std_Char_Stats m_tStd_Char_Stats;      // �⺻ ĳ���� ���� (���ذ�)
    // ���� ���� (���� + ������ + �ɷ�ġ ��)
    // ========================

    // �ΰ��� ����
    // =========
    ER_Ingame_Stats m_tIngame_Stats;    // �ΰ��� ����
    // ����/����� ��
    
    // ======[���ۺ� �Լ�]======
    // �Ϲ�

    // �⺻ ����
    // ��ų Q
    // ��ų W
    // ��ų E
    // ��ų R
    
    // ����
    // ä��
    
    // ����

    // ����
    // ���
    
    // �޽�

    // �̵�
    // ========================


public:
    void begin();

    void tick();
    virtual void finaltick();
    void finaltick_module();
    void render();
    void render_shadowmap();

};