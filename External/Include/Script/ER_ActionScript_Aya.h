#pragma once
#include "ER_ActionScript_Character.h"

class ER_ActionScript_Aya :
    public ER_ActionScript_Character
{
protected:
    virtual FSMState* CreateWait();
    virtual FSMState* CreateMove();
    virtual FSMState* CreateCraft();
    virtual FSMState* CreateRest();
    virtual FSMState* CreateAttack();
    virtual FSMState* CreateArrive();
    virtual FSMState* CreateDeath();
    virtual FSMState* CreateSkill_Q();
    virtual FSMState* CreateSkill_W();
    virtual FSMState* CreateSkill_E();
    virtual FSMState* CreateSkill_R();

public:
    virtual void Attack(CGameObject* _EnemyObj);                // �⺻����
    virtual void Wait();                                        // ���
    virtual void Move(CGameObject* _Target, Vec3 _DestPos);     // �̵�
    virtual void Craft();                                       // ����
    virtual void Rest();                                        // �޽�
    virtual void Skill_Q(CGameObject* _Target, Vec3 _DestPos);  // Q Skill
    virtual void Skill_W(CGameObject* _Target, Vec3 _DestPos);  // W Skill
    virtual void Skill_E(CGameObject* _Target, Vec3 _DestPos);  // E Skill
    virtual void Skill_R(CGameObject* _Target, Vec3 _DestPos);  // R Skill

public:
    ER_ActionScript_Aya();
    ~ER_ActionScript_Aya();

    CLONE(ER_ActionScript_Aya);
};

