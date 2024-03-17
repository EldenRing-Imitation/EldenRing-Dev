#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"

ER_ActionScript_Rio::ER_ActionScript_Rio()
    :ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
{
}

ER_ActionScript_Rio::~ER_ActionScript_Rio()
{
}

FSMState* ER_ActionScript_Rio::CreateWait()
{
    FSMState* state = new FSMState(this);

    // state->SetStateEnter((SCRIPT_DELEGATE)&ER_ActionScript_Rio::WaitEnter);
    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Wait);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Move);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Move);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Rest);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Rest);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateArrive()
{
    FSMState* state = new FSMState(this);
    
    STATEDELEGATE_ENTER(state, ER_ActionScript_Rio, Arrive);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Rio, Arrive);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    return state;
}

void ER_ActionScript_Rio::begin()
{
    ER_ActionScript_Character::begin();
    ChangeState(ER_CHAR_ACT::ARRIVE);
}

void ER_ActionScript_Rio::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Rio::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Rio::Skill_Q(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Skill_E(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::Skill_R(tFSMData& _Data)
{
}

void ER_ActionScript_Rio::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Run");
    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    bool bMove = findpathcomp->FindPath(DestPos);

    if (!bMove)
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::MoveUpdate(tFSMData& param)
{
    // ĳ���� �ӵ� ���ͼ� �־��ֱ�
    float speed = m_Data->GetStatus().fMovementSpeed;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
} 

void ER_ActionScript_Rio::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Wait");
    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Rio::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Arrive", false);
}

void ER_ActionScript_Rio::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Rio::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Rio_Rest_Start", false);
    
    param.iData = 0;
}

void ER_ActionScript_Rio::RestUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();
    
    switch (param.iData)
    {
    case 0: // ���� ����
    {
        // ���� ĵ������
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Rio_Rest_Loop");
            
            // ���º���Ұ�
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
        }
        break;
    }
    case 1: // ���� ��
    {
        // ĵ�� �Ұ�
        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            animator->SelectAnimation(L"Rio_Rest_End", false);
            param.iData++;
        }
        break;
    }
    case 2: // ���� ����
    {
        // ĵ�� ����
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���
        if (animator->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData = 0;
        }
        break;
    }
    }
}
