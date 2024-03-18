#pragma once
#include <Engine\CScript.h>

class FSM;
class FSMState;
class ER_DataScript_Character;

class ER_ActionScript_Character :
    public CScript
{
public:
    enum eCharacterActionState
    {
        WAIT,       // ���
        MOVE,       // �̵�
        CRAFT,      // ����
        REST,       // �޽�
        ATTACK,     // ����
        ARRIVE,     // ����
        DEATH,      // ���
        SKILL_Q,    // Q
        SKILL_W,    // W
        SKILL_E,    // E
        SKILL_R,    // R
        END,
    }typedef ER_CHAR_ACT;

    enum bAbleChange
    {
        COMMON,     // �Ϲݸ��
        ABSOUTE,    // ĵ����
        DISABLE,    // ����Ұ�
    };


protected:
    FSM* FSMContext;
    FSMState* StateList[ER_CHAR_ACT::END];  // State List
    UINT m_iPrevState;                      // Prev State
    UINT m_iCurState;                       // Cur State
    ER_DataScript_Character* m_Data;        // Cur Character Data

    bAbleChange m_bAbleChange;              // ���� ���� ���� ����

    // FSMState�� Delegate�� �����ؼ� �������ִ� �Լ�
    virtual FSMState* CreateWait() = 0;
    virtual FSMState* CreateMove() = 0;
    virtual FSMState* CreateCraft() = 0;
    virtual FSMState* CreateRest() = 0;
    virtual FSMState* CreateAttack() = 0;
    virtual FSMState* CreateArrive() = 0;
    virtual FSMState* CreateDeath() = 0;
    virtual FSMState* CreateSkill_Q() = 0;
    virtual FSMState* CreateSkill_W() = 0;
    virtual FSMState* CreateSkill_E() = 0;
    virtual FSMState* CreateSkill_R() = 0;

protected:
    Vec3 GetFocusPoint();                // Ÿ�� ����
    Vec3 GetClearPoint(const Vec3& vDir, float dist);
    float GetClearDistance(const Vec3& vDir, float dist);
    float GetClearDistanceByWall(const Vec3& vDir, float dist);
    Vec3 SetRotationToTarget(const Vec3& vTarget);
    Vec3 GetFocusPoint();


public:
    virtual void begin() override;
    virtual void tick() override;

    // ���� ��� �Լ�. FSM ���� ��� �� �߰� ���� ����
    virtual void Attack(tFSMData& _Data) = 0;   // �⺻����
    virtual void Wait(tFSMData& _Data);         // ���
    virtual void Move(tFSMData& _Data);         // �̵�
    virtual void Craft(tFSMData& _Data) {}      // ����
    virtual void Rest(tFSMData& _Data) {}       // �޽�
    virtual void Skill_Q(tFSMData& _Data) = 0;  // Q Skill
    virtual void Skill_W(tFSMData& _Data) = 0;  // W Skill
    virtual void Skill_E(tFSMData& _Data) = 0;  // E Skill
    virtual void Skill_R(tFSMData& _Data) = 0;  // R Skill

    void StateInit();

    bool ChangeState(ER_CHAR_ACT _state, bAbleChange _Grade = bAbleChange::COMMON);
    
    void SetAbleToCancle(bAbleChange _Grade) { m_bAbleChange = _Grade; }
    bool IsAbleChange(bAbleChange _Grade);

public:
    ER_ActionScript_Character(SCRIPT_TYPE _type);
    ~ER_ActionScript_Character();

    CLONE_DISABLE(ER_ActionScript_Character);
};