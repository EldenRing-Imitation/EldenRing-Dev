#pragma once
#include <Engine\CScript.h>

class ER_ActionScript_Character;
class ER_DataScript_Character;

class ER_PlayerScript :
    public CScript
{
private:
    ER_DataScript_Character* m_Character;
    ER_ActionScript_Character* m_pActionScript;
    CGameObject* m_pRangeObject;

    bool        m_AttackCsr;

private:
    Vec3            GetFocusPoint();                // Ÿ�� ����
    std::pair<CGameObject*, int>    GetFocusObj();                  // ���콺���̿� �浹���� ������Ʈ
    Vec3            GetFocusDir(Vec3 _Point);       // ���콺���� ����

public:
    void init();
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetRangeObject(CGameObject* _obj)
    {
        m_pRangeObject = _obj;
    }

public:
    ER_PlayerScript();
    ~ER_PlayerScript();

    CLONE_DISABLE(ER_PlayerScript);
};

