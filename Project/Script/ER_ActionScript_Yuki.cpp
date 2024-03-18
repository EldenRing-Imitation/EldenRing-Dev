#include "pch.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_DataScript_Character.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}

FSMState* ER_ActionScript_Yuki::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Wait);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Move);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Move);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateCraft()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Rest);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Rest);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Arrive);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Arrive);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateDeath()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Skill_Q);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Skill_W);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Skill_E);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, ER_ActionScript_Yuki, Skill_R);
    STATEDELEGATE_UPDATE(state, ER_ActionScript_Yuki, Skill_R);

    return state;
}

void ER_ActionScript_Yuki::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Wait(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Yuki::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Yuki::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Yuki::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Yuki::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Yuki::Skill_E(tFSMData& _Data)
{
    // �������� �ƴ϶��
    if (!STATEDATA_GET(SKILL_E).iData)
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Yuki::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Yuki::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Run", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Yuki::MoveUpdate(tFSMData& param)
{
    // ĳ���� �ӵ� ���ͼ� �־��ֱ�
    float speed = m_Data->GetStatus().fMovementSpeed;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Yuki::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Arrive", false);
}

void ER_ActionScript_Yuki::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Yuki::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Yuki_Rest_Loop");

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
            animator->SelectAnimation(L"Yuki_Rest_End", false);
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

void ER_ActionScript_Yuki::Skill_QEnter(tFSMData& param)
{
    // ��ȭ ��Ÿ ����
    tFSMData AttackData = STATEDATA_GET(ATTACK);
    AttackData.iData = 1;
    STATEDATA_SET(ATTACK, AttackData);
}

void ER_ActionScript_Yuki::Skill_QUpdate(tFSMData& param)
{
    // ���� ������ ���½�ų.

    SetAbleToCancle(bAbleChange::COMMON);

    if (ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_WEnter(tFSMData& param)
{
    // GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillW_Upper_Wait", false);
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillE_Attack", false);

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Yuki::Skill_WUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
};

void ER_ActionScript_Yuki::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Yuki_SkillE_Move", false);

    // ���� ��ȯ
    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 5.f;                                                      // ��ų �Ÿ�
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                                // �̵� ���� �Ÿ�
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;    // ��ü �ִϸ��̼� ��� �ð�
    param.v4Data[3] = 0.f;                                                      // �̵��� �Ÿ� �ʱ�ȭ.

    param.iData = 1;                                                    // �� ��ü �浹����

    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Yuki::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();

    switch (param.iData)
    {
    case 1: // �̵�
    {
        // 4frm ���ĺ��� �̵�
        if (GetOwner()->Animator3D()->GetCurFrame() < 4)
            return;
        
        
        // ����ü �浹�Ǵ�
        {
            // ���Ƿ� �̵��Ÿ��� 80% �������� �浹�ߴٰ� ���� : TEST �ڵ�
            if (param.v4Data[1] * 0.8 < param.v4Data[3])
            {
                GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillE_Attack", false);
                param.iData++;
                return;
            }
        }

        // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
        if (param.v4Data[3] < param.v4Data[1])
        {

            // param.v4Data[0] : ��ų �Ÿ�(�ӵ�)
            // param.v4Data[1] : �̵� ���� �Ÿ�
            // param.v4Data[2] : ��ü �ִϸ��̼� ��� �ð�
            // param.v4Data[3] : �̵��� �Ÿ�

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 4;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            transform->SetRelativePos(vPos);
        }

        // �̵����� ���� �浹�� �߻����� �ʰ� �ִϸ��̼� ����.
        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT,bAbleChange::DISABLE);

        break;
    }
    case 2: // ���� �ִϸ��̼�
    {
        Vec3 vPos = transform->GetRelativePos();

        // 4m �߰��̵����� ����
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        param.v4Data[0] = 0.5f;                                              // ��ų �Ÿ�
        float ClearDist = GetClearDistance(vDir, param.v4Data[0]);

        vPos += vDir * ClearDist;
        transform->SetRelativePos(vPos);

        param.iData++;
        break;
    }
    case 3: // �ִϸ��̼� ����
    {
        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, bAbleChange::DISABLE);
        
        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_EExit(tFSMData& param)
{
    param.iData = 0;
}

void ER_ActionScript_Yuki::Skill_REnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    param.iData = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ��¡ ����, 3. ��ų ǥ�� ����
}

void ER_ActionScript_Yuki::Skill_RUpdate(tFSMData& param)
{
    switch (param.iData)
    {
    case 1:
    {
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::R))
        {
            // ��ų ����
            Vec3 vTargetPoint = GetFocusPoint();
            SetRotationToTarget(vTargetPoint);

            // ��ų �ߵ�
            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_Loop", false);
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
        }
        break;
    }
    case 2:
    {
        // ��ų Ÿ�ݹ��� �ִϸ��̼� ���

        // Ư�� �����ӿ� ���� ��ǰ� �Բ� �����Ŵ���ȣ��

        if (GetOwner()->Animator3D()->IsFinish())
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_End", false);
            param.iData++;
        }
        break;
    }
    case 3:
    {

        // Ư�� �����ӿ� ���� ��ǰ� �Բ� ���� �Ŵ��� ȣ��

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, bAbleChange::DISABLE);

        break;
    }
    default:
        break;
    }
}

void ER_ActionScript_Yuki::Skill_RExit(tFSMData& param)
{
    // ��ų���� ������ ����
    param.iData = 0;
}
