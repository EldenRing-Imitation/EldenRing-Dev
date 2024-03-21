#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"
#include "ER_ProjectilePool.h"
#include "ER_ProjectileScript.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Rio::ER_ActionScript_Rio()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
    , m_BowType(false)
{
}

ER_ActionScript_Rio::~ER_ActionScript_Rio()
{
}

FSMState* ER_ActionScript_Rio::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Wait);
    STATEDELEGATE_UPDATE(state, Rio, Wait);
    STATEDELEGATE_EXIT(state, Rio, Wait);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Move);
    STATEDELEGATE_UPDATE(state, Rio, Move);
    STATEDELEGATE_EXIT(state, Rio, Move);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Craft);
    STATEDELEGATE_UPDATE(state, Rio, Craft);
    STATEDELEGATE_EXIT(state, Rio, Craft);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Rest);
    STATEDELEGATE_UPDATE(state, Rio, Rest);
    STATEDELEGATE_EXIT(state, Rio, Rest);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Attack);
    STATEDELEGATE_UPDATE(state, Rio, Attack);
    STATEDELEGATE_EXIT(state, Rio, Attack);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateArrive()
{
    FSMState* state = new FSMState(this);
    
    STATEDELEGATE_ENTER(state, Rio, Arrive);
    STATEDELEGATE_UPDATE(state, Rio, Arrive);
    STATEDELEGATE_EXIT(state, Rio, Arrive);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Dead);
    STATEDELEGATE_UPDATE(state, Rio, Dead);
    STATEDELEGATE_EXIT(state, Rio, Dead);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_Q);
    STATEDELEGATE_UPDATE(state, Rio, Skill_Q);
    STATEDELEGATE_EXIT(state, Rio, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_W);
    STATEDELEGATE_UPDATE(state, Rio, Skill_W);
    STATEDELEGATE_EXIT(state, Rio, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_E);
    STATEDELEGATE_UPDATE(state, Rio, Skill_E);
    STATEDELEGATE_EXIT(state, Rio, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Rio::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Skill_R);
    STATEDELEGATE_UPDATE(state, Rio, Skill_R);
    STATEDELEGATE_EXIT(state, Rio, Skill_R);

    return state;
}

void ER_ActionScript_Rio::Attack(tFSMData& _Data)
{
    // ���� �������°� �ƴ�
    if (!_Data.bData[0])
    {
        // ���� �������� ��ȯ
        _Data.bData[0] = true;
    }

    STATEDATA_SET(ATTACK, _Data);

    CGameObject* TargetObj = (CGameObject*)_Data.lParam;
    float AtkRange = m_Data->GetStatus()->fAtakRange;
    if (IsInRange(TargetObj, AtkRange))
    {
        _Data.bData[0] = false;
        ChangeState(ER_CHAR_ACT::ATTACK);
    }
    else
    {
        // �����Ÿ� ���̱⶧���� Ÿ�ٹ������� �̵��Ѵ�.
        tFSMData MoveData = STATEDATA_GET(MOVE);
        MoveData.v4Data = TargetObj->Transform()->GetRelativePos();
        Move(MoveData);
    }
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
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Rio::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Rio::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Rio::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Rio::MoveEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Run", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Rio::MoveUpdate(tFSMData& param)
{
    tFSMData Atkdata = STATEDATA_GET(ATTACK);

    // �����������¶��
    if (Atkdata.bData[0])
    {
        CGameObject* TargetObj = (CGameObject*)Atkdata.lParam;
        float AtkRange = m_Data->GetStatus()->fAtakRange;

        if (IsInRange(TargetObj, AtkRange))
        {
            Atkdata.bData[0] = false;
            GetOwner()->FindPath()->ClearPath();
            ChangeState(ER_CHAR_ACT::ATTACK, bAbleChange::DISABLE);
            return;
        }
    }

    // ĳ���� �ӵ� ���ͼ� �־��ֱ�
    tStatus_Effect* statusefc = m_Data->GetStatusEffect();
    float speed = m_Data->GetStatus()->fMovementSpeed;
    speed += (speed * statusefc->GetIncSPD()) - (speed * statusefc->GetDecSPD());


    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Rio::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::CraftEnter(tFSMData& param)
{
}

void ER_ActionScript_Rio::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Rio::CraftExit(tFSMData& param)
{
}


void ER_ActionScript_Rio::WaitEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Wait", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Wait", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Rio::WaitUpdate(tFSMData& param)
{
}

void ER_ActionScript_Rio::WaitExit(tFSMData& param)
{
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

void ER_ActionScript_Rio::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::AttackEnter(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[2] = false;

    // Ȱ ��
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Attack", false);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Attack", false);


    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
}

void ER_ActionScript_Rio::AttackUpdate(tFSMData& param)
{
    // bData[0] : ���� ��� ��������
    // bData[1] : -
    // bData[2] : Hit���� ���࿩��
    // bData[3] : -

    CAnimator3D* animator = GetOwner()->Animator3D();
    float Atkspd = m_Data->GetStatus()->fAttackSpeed;
    tStatus_Effect* statusefc = m_Data->GetStatusEffect();
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // �ִϸ��̼� �ӵ� ����
    animator->PlaySpeedValue(Atkspd);


     // ��������
    int HitFrame = m_BowType ? 8 : 8;
    if (!param.bData[2] && animator->GetCurFrame() < HitFrame)
    {
        ER_ProjectileScript* Arrow = ER_ProjectilePool::GetInst()->GetProjectile(ER_ProjectilePool::eProjType::ARROW);
        
        Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
        Vec3 vUP = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP).Normalize();
        Vec3 vFront = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT).Normalize();

        vPos += vUP * 0.2f;
        vPos += vFront * 0.2f;

        Arrow->ShotTarget(GetOwner(), (CGameObject*)param.lParam, vPos, ER_ProjectileScript::eDmgType::COMMON, 20.f);
        Arrow->Shot();

        BATTLE_COMMON(GetOwner(), param.lParam);
        param.bData[2] = true;
    }


    if (animator->IsFinish())
    {
        CGameObject* Target = (CGameObject*)param.lParam;
        // ��� �Ǵ�
        bool IsDead = Target->GetScript<ER_DataScript_Character>()->IsDeadState();

        if (IsDead)
            ChangeState(ER_CHAR_ACT::WAIT);
        else
        {
            // �Ÿ� �Ǵ�
            float AtkRange = m_Data->GetStatus()->fAtakRange;

            if (IsInRange(Target, AtkRange))
                AttackEnter(param);
            else
                Attack(param);
        }
    }
}

void ER_ActionScript_Rio::AttackExit(tFSMData& param)
{
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

void ER_ActionScript_Rio::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::Skill_QEnter(tFSMData& param)
{
    // Bow Ÿ�� ��ȯ
    m_BowType = !m_BowType;

    if(m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Skill_Q", false);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_Q", false);

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_QUpdate(tFSMData& param)
{
    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
    {
        if (7 <= GetOwner()->Animator3D()->GetCurFrame())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::MOVE);
        }
    }
    else if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::Skill_WEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    if (m_BowType)
        Animator->SelectAnimation(L"Rio_Long_Skill_W", false);
    else
        Animator->SelectAnimation(L"Rio_Short_Skill_W", false);

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_WUpdate(tFSMData& param)
{
    // 11�����ӿ� ĵ���������� ���� (�ĵ����̻���)
    if (11 == GetOwner()->Animator3D()->GetCurFrame())
        SetAbleToCancle(bAbleChange::COMMON);

    // ��ų �ִϸ��̼��� ������ �����·� �ڵ� ��ȯ.
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::Skill_EEnter(tFSMData& param)
{
    // �ܱ����� ����� �������
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Rio_Short_Skill_E", false);
    
    // ���� ��ȯ
    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();
    
    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;
    
    param.v4Data[0] = 10.f;                                              // ��ų �Ÿ�
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // �̵� ���� �Ÿ�
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;   // ��ü �ִϸ��̼� ��� �ð�
    param.v4Data[3] = 0.f;                                              // �̵��� �Ÿ� �ʱ�ȭ.

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Rio::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    // param.v4Data[0] : ��ų �Ÿ�(�ӵ�)
    // param.v4Data[1] : �̵� ���� �Ÿ�
    // param.v4Data[2] : ��ü �ִϸ��̼� ��� �ð�
    // param.v4Data[3] : �̵��� �Ÿ�

    if (animator->GetCurFrame() < 3)
    {
        if (param.v4Data[3] < param.v4Data[1] * 0.1f)
        {
            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 0.5f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            transform->SetRelativePos(vPos);
        }
        return;
    }
    // 3 ����
    else if (animator->GetCurFrame() < 16)
    {
        // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
        if (param.v4Data[3] < param.v4Data[1])
        {
            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 1.5f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            transform->SetRelativePos(vPos);
        }
        return;
    }
    // 17 ����

    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        // clear
        param.v4Data = Vec4();
        param.v4Data = Vec2();
     
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Rio::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::Skill_REnter(tFSMData& param)
{
    if (m_BowType)
    {
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Skill_R", false);
        param.iData = 0;
    }
    else
    {
        // ù ����
        if(0 == param.iData)
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_Start", false);

        // �ι�° ����
        if(1 == param.iData)
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_End", false);
    }

    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Rio::Skill_RUpdate(tFSMData& param)
{
    // ��� ��
    if (m_BowType)
    {
        if (GetOwner()->Animator3D()->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }
    // �ܱ� ��
    else
    {
        CAnimator3D* animator = GetOwner()->Animator3D();

        switch (param.iData)
        {
        case 0: // ���� ����
        {
            // �ִϸ��̼� ���̸�ŭ ���������� UI ���

            // 3����

            if (animator->IsFinish())
            {
                SetAbleToCancle(bAbleChange::COMMON);
                ChangeState(ER_CHAR_ACT::WAIT);
                param.iData++;
            }
            break;
        }
        case 1: // ���ᵿ��
        {
            // �ִϸ��̼� ���̸�ŭ ���������� UI ���

            // 1��

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
}

void ER_ActionScript_Rio::Skill_RExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::DeadEnter(tFSMData& param)
{
}

void ER_ActionScript_Rio::DeadUpdate(tFSMData& param)
{
}

void ER_ActionScript_Rio::DeadExit(tFSMData& param)
{
}
