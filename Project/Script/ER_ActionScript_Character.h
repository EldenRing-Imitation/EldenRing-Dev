#pragma once
#include <Engine\CScript.h>
#include <Engine\CSound.h>
#include <Engine\ptr.h>

class FSM;
class FSMState;
class ER_DataScript_Character;

class ER_ActionScript_Character :
    public CScript
{
public:
    enum class eCharacterActionState
    {
        WAIT,       // ���
        MOVE,       // �̵�
        FARMING,    // �Ĺ�
        CRAFT,      // ����
        REST,       // �޽�
        ATTACK,     // ����
        ARRIVE,     // ����
        DEAD,       // ���
        SKILL_Q,    // Q
        SKILL_W,    // W
        SKILL_E,    // E
        SKILL_R,    // R
        END,
    }typedef ER_CHAR_ACT;

    enum class eAccessGrade
    {
        BASIC,          // �Ϲ� ��� ����
        ADVANCED,       // ���� ��� ����
        UTMOST,         // �ְ� ��� ����
    };


protected:
    FSM*                        FSMContext;
    FSMState*                   StateList[(UINT)ER_CHAR_ACT::END];  // State List
    UINT                        m_iPrevState;                       // Prev State
    UINT                        m_iCurState;                        // Cur State
    ER_DataScript_Character*    m_Data;                             // Cur Character Data
    eAccessGrade                m_AccessGrade;                      // ���� ���� ���� ���

    // Create State
    virtual FSMState* CreateWait() = 0;
    virtual FSMState* CreateArrive() = 0;
    virtual FSMState* CreateDead() = 0;

    virtual FSMState* CreateAttack() = 0;
    virtual FSMState* CreateMove() = 0;
    virtual FSMState* CreateFarming() = 0;
    virtual FSMState* CreateCraft() = 0;
    virtual FSMState* CreateRest() = 0;
    
    virtual FSMState* CreateSkill_Q() = 0;
    virtual FSMState* CreateSkill_W() = 0;
    virtual FSMState* CreateSkill_E() = 0;
    virtual FSMState* CreateSkill_R() = 0;

    virtual bool SoundLoad() = 0;

    // [ Status Func ]
    tIngame_Stats* GetStatus();                                     // ĳ���� ���� Ȯ��
    tStatus_Effect* GetStatusEffect();                              // ĳ���� ����ȿ�� Ȯ��
    ER_DataScript_Character* GetCharacterData();                    // ĳ���� �����ͽ�ũ��Ʈ Ȯ��

    bool IsSkillOn(SKILLIDX _idx);                                  // ���ӽ�ų ���� �Ǵ�

    // [ Transform Func ]
    Vec3 GetFocusPoint();                                           // Ÿ�� ��ǥ Ȯ��
    Vec3 GetFocusDir();                                             // Ÿ�� ���� Ȯ��
    Vec3 GetClearPoint(const Vec3& vDir, float dist);               // �̵� ���� ���� Ȯ��
    float GetClearDistance(const Vec3& vDir, float dist);           // �̵� ���� �Ÿ� Ȯ��
    float GetClearDistanceByWall(const Vec3& vDir, float dist);     // ������ �̵������� �Ÿ� Ȯ��
    Vec3 SetRotationToTarget(const Vec3& vTarget);                  // Ÿ�� ���� ȸ��

    bool IsInRange(CGameObject* Target, float _fRange);             // �Ÿ����� Ȯ��
    bool IsInRangeWithAngle(CGameObject* _Owner, CGameObject* _Target, float _fRange, float _degree);
    Vec3 GetProjSpawnPos(DWORD_PTR _Target);

    // [ State Manage ]
    void StateInit();                                                                // ���� �ʱ�ȭ ����
    bool ChangeState(ER_CHAR_ACT _state, eAccessGrade _Grade = eAccessGrade::BASIC); // ���� ����
    void SetStateGrade(eAccessGrade _Grade) { m_AccessGrade = _Grade; }              // ���� ���� ���� ���
    bool IsAbleChange(eAccessGrade _Grade);                                          // ���� ��� ��


    // [ Sound ]
    virtual void PlaySound(UINT _SoundKey) {}

    // [ Player Check ]
    bool IsPlayer();
    bool IsCharacter(CGameObject* _Obj);
    bool IsDead(CGameObject* _Obj);


public:
    virtual void begin() override;
    virtual void tick() override;

    ER_CHAR_ACT GetCurState() { return (ER_CHAR_ACT)m_iCurState; }
    bool IsThisState(ER_CHAR_ACT _State) { return _State == (ER_CHAR_ACT)m_iCurState; }

    // ���� �Լ�
    virtual void Attack(tFSMData& _Data);       // �⺻����
    virtual void Wait(tFSMData& _Data);         // ���
    virtual void Move(tFSMData& _Data);         // �̵�
    virtual void Farming(tFSMData& _Data);      // �������Ĺ�
    virtual void Craft(tFSMData& _Data);        // ����
    virtual void Rest(tFSMData& _Data);         // �޽�
    virtual void Dead(tFSMData& _Data);         // ���
    
    // ���� ���� �Լ�
    virtual void Skill_Q(tFSMData& _Data) = 0;  // Q Skill
    virtual void Skill_W(tFSMData& _Data) = 0;  // W Skill
    virtual void Skill_E(tFSMData& _Data) = 0;  // E Skill
    virtual void Skill_R(tFSMData& _Data) = 0;  // R Skill

public:
    ER_ActionScript_Character(SCRIPT_TYPE _type);
    ~ER_ActionScript_Character();

    CLONE_DISABLE(ER_ActionScript_Character);
};