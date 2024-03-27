#include "pch.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Jackie::ER_ActionScript_Jackie()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE)
{
}

ER_ActionScript_Jackie::~ER_ActionScript_Jackie()
{
}

void ER_ActionScript_Jackie::WaitEnter(tFSMData& param)
{
    /*
    bData[0] = �ñر� ������ �Ǵ�
    */
    
    // �ñر� �۵� ����
    param.bData[0] = IsSkillOn(SKILLIDX::R_1);

    if (param.bData[0])
        Animator3D()->SelectAnimation(L"Jackie_R_Wait", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Jackie::WaitUpdate(tFSMData& param)
{
    /*
    bData[0]    = �ñر� �������̾����� �Ǵ�
    */
    
    // 0.5�ʸ��� ü��ȸ��
    param.fData += DT;

    if (0.5f <= param.fData)
    {
        // HP/SP �ڿ� ȸ��
        m_Data->HPRegen();
        m_Data->SPRegen();

        // ü����� ī��Ʈ �ʱ�ȭ
        param.fData -= 0.5f;
    }

    // ��ų ���ӽð��� ����Ȱ��
    bool IsAction = IsSkillOn(SKILLIDX::R_1);
    if (param.bData[0] && !IsAction)
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);
}
void ER_ActionScript_Jackie::WaitExit(tFSMData& param)
{
}


void ER_ActionScript_Jackie::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    bData[1]    : �̵��ӵ� ������ų �ۿ� ����
    fData		: ���� ���� �Ÿ� / �Ĺ� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ�� / 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */
    
    param.bData[1] = IsSkillOn(SKILLIDX::W_1);

    if(param.bData[1])
        Animator3D()->SelectAnimation(L"Jackie_SkillW", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Run", true);

    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Jackie::MoveUpdate(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    bData[1]    : �̵��ӵ� ������ų �ۿ� ����
    fData		: ���� ���� �Ÿ� / �Ĺ� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ�� / 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */

    // Ÿ�� ������
    if (param.bData[0])
    {
        if (IsInRange((CGameObject*)param.lParam, param.fData))
        {
            param.bData[0] = false;     // ���� ����    
            FindPath()->ClearPath();    // �̵� ��� �ʱ�ȭ
            switch (param.iData[0])
            {
            case 1: // ���� ���
                ChangeState(ER_CHAR_ACT::ATTACK);
                break;
            case 2: // ������ �ڽ�
                ChangeState(ER_CHAR_ACT::FARMING);
                break;
            }
            return; // ������ȯ �� �۾� �Ϸ�
        }
    }

    // �ִϸ��̼� ���� �Ǵ�
    bool IsAction = IsSkillOn(SKILLIDX::W_1);
    if (param.bData[1] && !IsAction)
        Animator3D()->SelectAnimation(L"Jackie_Run", true);

    // ����/����� ȿ�� �ݿ�
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // �̵��ӵ� ����
    float fMoveSpeed = GetStatus()->fMovementSpeed;
    fMoveSpeed += (fMoveSpeed * SpeedEfc->GetIncSPD()) - (fMoveSpeed * SpeedEfc->GetDecSPD());

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!FindPath()->PathMove(fMoveSpeed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Jackie::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::FarmingEnter(tFSMData& param)
{
    param.bData[0] = IsSkillOn(SKILLIDX::R_1);

    if (param.bData[0])
        Animator3D()->SelectAnimation(L"Jackie_R_Wait", true);
    else
        Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);

}

void ER_ActionScript_Jackie::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Jackie::RestEnter(tFSMData& param)
{
    /*
    iData[0] = �޽� �ִϸ��̼� ����Ǵ�
    fData    = ü������ð� ī��Ʈ
    */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Jackie_Rest_Start", false);
}
void ER_ActionScript_Jackie::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Jackie_Rest_Loop");

            // ���º���Ұ�
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
        }
        break;
    }
    case 1: // ���� ��
    {
        // ĵ�� �Ұ�
        // 0.5�ʸ��� ȸ��
        param.fData += DT;

        if (0.5f <= param.fData)
        {
            // HP/SP �ڿ� ȸ�� 5�� ������ ȸ��
            m_Data->HPRegen(5.f);
            m_Data->SPRegen(5.f);

            // �ڿ���� ī��Ʈ �ʱ�ȭ
            param.fData -= 0.5f;
        }

        if (KEY_TAP(KEY::RBTN) || KEY_TAP(KEY::X))
        {
            Animator3D()->SelectAnimation(L"Jackie_Rest_End", false);
            param.iData[0]++;
        }
        break;
    }
    case 2: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���
        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    }
}
void ER_ActionScript_Jackie::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Arrive", false);
}
void ER_ActionScript_Jackie::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Jackie::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Death", false);
}
void ER_ActionScript_Jackie::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Jackie::DeadExit(tFSMData& param)
{
}


void ER_ActionScript_Jackie::AttackEnter(tFSMData& param)
{
    /*
    [ATTACK]
    bData[0]	    : ���ݵ��� ���������� ����
    bData[1]	    : Battle Event ���� ����
    bData[2]	    : ���� Ÿ�� ���� ����
    bData[3]        : ���ݸ�� ����
    iData[0]	    : Ÿ������ �ִϸ��̼� ������ = Hit Frame
    lParam		    : Ÿ�� ������Ʈ
    RParam		    : Ÿ�� ���� ������Ʈ
    */

    // ���� ���۴ܰ� �ʱ�ȭ
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    if (IsSkillOn(SKILLIDX::R_1))
    {
        if (param.bData[3])
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack0", false);
            param.iData[0] = 6;
        }
        else
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_R_Attack1", false);
            param.iData[0] = 6;
        }
    }
    else
    {
        if (param.bData[3])
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack0", false);
            param.iData[0] = 6;
        }
        else
        {
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Attack1", false);
            param.iData[0] = 6;
        }
    }

    // Ÿ�ٹ������� ȸ��
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Jackie::AttackUpdate(tFSMData& param)
{
    /*
     [ATTACK]
     bData[0]	: ���ݵ��� ���������� ����
     bData[1]	: Battle Event ���� ����
     bData[2]	: ���� Ÿ�� ���� ����
     bData[3]    : ���ݸ�� ����
     iData[0]	: Ÿ������ �ִϸ��̼� ������ = Hit Frame
     lParam		: Ÿ�� ������Ʈ
     RParam		: Ÿ�� ���� ������Ʈ
     */

    float Atkspd = GetStatus()->fAttackSpeed;

    // ����/����� Ȯ��
    tStatus_Effect* statusefc = GetStatusEffect();
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // �ִϸ��̼� �ӵ� ����
    Animator3D()->PlaySpeedValue(Atkspd);

    if (!param.bData[1] && param.iData[0] < Animator3D()->GetCurFrame())
    {
        // ���� ���

        // ĳ���� ���� ���� �˰���

        if (IsSkillOn(SKILLIDX::R_1))
        {
            // ��ȭ��Ÿ�� ��������
            // ��ų ������ 
            // ��Ÿ ������
        }
        else
            BATTLE_COMMON(GetOwner(), param.lParam);

        param.bData[1] = true;
        SetStateGrade(eAccessGrade::BASIC);
    }


    if (Animator3D()->IsFinish())
    {
        param.bData[0] = false;         // ���� ���� �Ϸ�

        // ������ Ÿ���� ����Ǿ���
        if (param.bData[2])
        {
            param.lParam = param.RParam;
            param.bData[2] = false;
            param.RParam = 0;
        }

        CGameObject* Target = (CGameObject*)param.lParam;

        // ��� �Ǵ�
        bool IsDead = Target->GetScript<ER_DataScript_Character>()->IsDeadState();

        if (IsDead)
            ChangeState(ER_CHAR_ACT::WAIT);
        else
        {
            // �Ÿ� �Ǵ�
            float AtkRange = GetStatus()->fAtkRange;

            if (IsInRange(Target, AtkRange))
                AttackEnter(param);
            else
                Attack(param);
        }
    }
}
void ER_ActionScript_Jackie::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Jackie::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Jackie_Craft", false);
    ERCHARSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Jackie::CraftUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
    {
        STOPSOUND(CRAFT_SOUND);
        // ������ �����Լ�
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        param.bData[0] = false;
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Jackie::CraftExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Jackie::Skill_W(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_W, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Jackie::Skill_E(tFSMData& _Data)
{
    // �������� �ƴ϶��
    if (!STATEDATA_GET(SKILL_E).bData[0])
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Jackie::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Jackie::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);

    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData[0] = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ����
}

void ER_ActionScript_Jackie::Skill_QUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 1:
    {
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::Q))
        {
            // ��ų ����
            Vec3 vTargetPoint = GetFocusPoint();
            SetRotationToTarget(vTargetPoint);

            // ��ų �ߵ�
            GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillQ", false);
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
        }
        break;
    }
    case 2:
    {

        // Ư�� �����ӿ� ���� ��ǰ� �Բ� ���� �Ŵ��� ȣ��

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);

        break;
    }
    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_QExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Jackie::Skill_WEnter(tFSMData& param)
{
    // ��ų���
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        float SpdValue = Skill->Float1();
        float Time = Skill->fActionTime;
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, Time, SpdValue);
    }
}

void ER_ActionScript_Jackie::Skill_WUpdate(tFSMData& param)
{
    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData[0] = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ����
}

void ER_ActionScript_Jackie::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    switch (param.iData[0])
    {
    case 1:
    {
        //����
        Vec3 vTargetPoint = GetFocusPoint();

        // �ִ�Ÿ� �̳��� ��ų����Ʈ�� ���� �������� �̵�.
        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::E))
        {
            animator->SelectAnimation(L"Jackie_SkillE_Start", false);

            // ��ų ����
            SetRotationToTarget(vTargetPoint);

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir = (vTargetPoint - vPos).Normalize();
            float fDist = Vec3::Distance(vPos, vTargetPoint);

            // ���������� v2Data�� �̵�
            param.v2Data.x = vDir.x;
            param.v2Data.y = vDir.z;

            float fMaxRange = 15.f; // �ִ� ��ų �Ÿ�

            param.v4Data[0] = fMaxRange < fDist ? fMaxRange : fDist;                                                      // ��ų �Ÿ�
            float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
            param.v4Data[1] = ClearDist;                                                // �̵� ���� �Ÿ�
            param.v4Data[2] = (float)animator->GetCurAnim()->GetAnimClip().dEndTime;    // ��ü �ִϸ��̼� ��� �ð�
            param.v4Data[3] = 0.f;

            // ��ų �ߵ�
            param.bData[0] = true;
            param.iData[0]++;
            SetStateGrade(eAccessGrade::UTMOST);
        }
        break;
    }
    case 2:
    {
        // 3frm ���ĺ��� �̵�
        if (GetOwner()->Animator3D()->GetCurFrame() < 3)
            return;

        //����
        if (param.v4Data[3] < param.v4Data[1])
        {

            // param.v4Data[0] : ��ų �Ÿ�(�ӵ�)
            // param.v4Data[1] : �̵� ���� �Ÿ�
            // param.v4Data[2] : ��ü �ִϸ��̼� ��� �ð�
            // param.v4Data[3] : �̵��� �Ÿ�

            Vec3 vPos = transform->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.v4Data[0] * 5.f;

            float CurFrameMoveDist = speed * param.v4Data[2] * DT;

            param.v4Data[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            transform->SetRelativePos(vPos);
        }

        if (GetOwner()->Animator3D()->IsFinish())
        {
            animator->SelectAnimation(L"Jackie_SkillE_End", false);
            param.iData[0]++;
        }

        break;
    }
    case 3:
    {
        // ���
        // 

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);

        break;
    }

    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_EExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Jackie::Skill_REnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    if (Skill->Use(&GetStatus()->iSP))
    {
        float SpdValue = (float)Skill->Int1();
        float Time = Skill->Float1();
    }

    param.bData[0] = 1;    // ��ų ON
}

void ER_ActionScript_Jackie::Skill_RUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_RExit(tFSMData& param)
{
}



FSMState* ER_ActionScript_Jackie::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Wait);
    STATEDELEGATE_UPDATE(state, Jackie, Wait);
    STATEDELEGATE_EXIT(state, Jackie, Wait);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Move);
    STATEDELEGATE_UPDATE(state, Jackie, Move);
    STATEDELEGATE_EXIT(state, Jackie, Move);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Farming);
    STATEDELEGATE_UPDATE(state, Jackie, Farming);
    STATEDELEGATE_EXIT(state, Jackie, Farming);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Craft);
    STATEDELEGATE_UPDATE(state, Jackie, Craft);
    STATEDELEGATE_EXIT(state, Jackie, Craft);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Rest);
    STATEDELEGATE_UPDATE(state, Jackie, Rest);
    STATEDELEGATE_EXIT(state, Jackie, Rest);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Attack);
    STATEDELEGATE_UPDATE(state, Jackie, Attack);
    STATEDELEGATE_EXIT(state, Jackie, Attack);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Arrive);
    STATEDELEGATE_UPDATE(state, Jackie, Arrive);
    STATEDELEGATE_EXIT(state, Jackie, Attack);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Dead);
    STATEDELEGATE_UPDATE(state, Jackie, Dead);
    STATEDELEGATE_EXIT(state, Jackie, Dead);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_Q);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_Q);
    STATEDELEGATE_EXIT(state, Jackie, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_W);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_W);
    STATEDELEGATE_EXIT(state, Jackie, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_E);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_E);
    STATEDELEGATE_EXIT(state, Jackie, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Skill_R);
    STATEDELEGATE_UPDATE(state, Jackie, Skill_R);
    STATEDELEGATE_EXIT(state, Jackie, Skill_R);

    return state;
}
