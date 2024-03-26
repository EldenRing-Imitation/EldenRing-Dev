#include "pch.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}


void ER_ActionScript_Yuki::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Yuki::WaitUpdate(tFSMData& param)
{
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
}
void ER_ActionScript_Yuki::WaitExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Yuki::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    bData[1]    : �̵��ӵ� ������ų �ۿ� ����
    fData		: ���� ���� �Ÿ� / �Ĺ� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ�� / 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */

    Animator3D()->SelectAnimation(L"Yuki_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Yuki::MoveUpdate(tFSMData& param)
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

    // ����/����� ȿ�� �ݿ�
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // �̵��ӵ� ����
    float fMoveSpeed = GetStatus()->fMovementSpeed;
    fMoveSpeed += (fMoveSpeed * SpeedEfc->GetIncSPD()) - (fMoveSpeed * SpeedEfc->GetDecSPD());

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!FindPath()->PathMove(fMoveSpeed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Yuki::MoveExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::FarmingEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}

void ER_ActionScript_Yuki::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Yuki::RestEnter(tFSMData& param)
{
    /*
    iData[0] = �޽� �ִϸ��̼� ����Ǵ�
    fData    = ü������ð� ī��Ʈ
    */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Yuki_Rest_Start", false);
}
void ER_ActionScript_Yuki::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Yuki_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Yuki_Rest_End", false);
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
void ER_ActionScript_Yuki::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Arrive", false);
}
void ER_ActionScript_Yuki::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Yuki::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Death", false);
}
void ER_ActionScript_Yuki::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Yuki::DeadExit(tFSMData& param)
{
}



void ER_ActionScript_Yuki::AttackEnter(tFSMData& param)
{
    /*
   [ATTACK]
   bData[0]	    : ���ݵ��� ���������� ����
   bData[1]	    : Battle Event ���� ����
   bData[2]	    : ���� Ÿ�� ���� ����
   bData[3]     : ���ݸ�� ����
   iData[0]	    : Ÿ������ �ִϸ��̼� ������ = Hit Frame
   lParam		: Ÿ�� ������Ʈ
   RParam		: Ÿ�� ���� ������Ʈ
   */

   // ���� ���۴ܰ� �ʱ�ȭ
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    // Q��ų ���� ����
    if (IsSkillOn(SKILLIDX::Q_1))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillQ", false);
        param.iData[0] = 6;
    }
    else if (param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack0", false);
        param.iData[0] = 6;
    }
    else
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack1", false);
        param.iData[0] = 6;
    }
    
    // Ÿ�ٹ������� ȸ��
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Yuki::AttackUpdate(tFSMData& param)
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
        
        if (IsSkillOn(SKILLIDX::Q_1))
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
void ER_ActionScript_Yuki::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Yuki::CraftEnter(tFSMData& param)
{
}

void ER_ActionScript_Yuki::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Yuki::CraftExit(tFSMData& param)
{
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


void ER_ActionScript_Yuki::Skill_QEnter(tFSMData& param)
{
    // ��ȭ ��Ÿ ����
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    Skill->Use(&GetStatus()->iSP, true);
}

void ER_ActionScript_Yuki::Skill_QUpdate(tFSMData& param)
{
    // ���� ������ ���½�ų.

    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::Skill_WEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    Skill->Use(&GetStatus()->iSP, true);

    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillW_Upper_Wait", false);
    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Yuki::Skill_WUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::Skill_WExit(tFSMData& param)
{
}
;

void ER_ActionScript_Yuki::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    Skill->Use(&GetStatus()->iSP);

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

    param.iData[0] = 1;                                                            // �� ��ü �浹����

    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Yuki::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();

    switch (param.iData[0])
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
                param.iData[0]++;
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
            ChangeState(ER_CHAR_ACT::WAIT,eAccessGrade::UTMOST);

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

        param.iData[0]++;
        break;
    }
    case 3: // �ִϸ��̼� ����
    {
        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, eAccessGrade::UTMOST);
        
        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_EExit(tFSMData& param)
{
    param.iData[0] = 0;
}

void ER_ActionScript_Yuki::Skill_REnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    param.iData[0] = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ��¡ ����, 3. ��ų ǥ�� ����
}

void ER_ActionScript_Yuki::Skill_RUpdate(tFSMData& param)
{
    switch (param.iData[0])
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
            SetStateGrade(eAccessGrade::UTMOST);
            param.iData[0]++;
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
            param.iData[0]++;
        }
        break;
    }
    case 3:
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

void ER_ActionScript_Yuki::Skill_RExit(tFSMData& param)
{
    // ��ų���� ������ ����
    param.iData[0] = 0;
}


int ER_ActionScript_Yuki::SkillQ(const tSkill_Info* skilldata)
{
    return skilldata->iValue1[0] + (int)skilldata->fValue1[0];
}


FSMState* ER_ActionScript_Yuki::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Wait);
    STATEDELEGATE_UPDATE(state, Yuki, Wait);
    STATEDELEGATE_EXIT(state, Yuki, Wait);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Move);
    STATEDELEGATE_UPDATE(state, Yuki, Move);
    STATEDELEGATE_EXIT(state, Yuki, Move);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Farming);
    STATEDELEGATE_UPDATE(state, Yuki, Farming);
    STATEDELEGATE_EXIT(state, Yuki, Farming);

    return state;
}

FSMState* ER_ActionScript_Yuki::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Craft);
    STATEDELEGATE_UPDATE(state, Yuki, Craft);
    STATEDELEGATE_EXIT(state, Yuki, Craft);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Rest);
    STATEDELEGATE_UPDATE(state, Yuki, Rest);
    STATEDELEGATE_EXIT(state, Yuki, Rest);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Attack);
    STATEDELEGATE_UPDATE(state, Yuki, Attack);
    STATEDELEGATE_EXIT(state, Yuki, Attack);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Arrive);
    STATEDELEGATE_UPDATE(state, Yuki, Arrive);
    STATEDELEGATE_EXIT(state, Yuki, Arrive);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Dead);
    STATEDELEGATE_UPDATE(state, Yuki, Dead);
    STATEDELEGATE_EXIT(state, Yuki, Dead);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_Q);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_Q);
    STATEDELEGATE_EXIT(state, Yuki, Skill_Q);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_W);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_W);
    STATEDELEGATE_EXIT(state, Yuki, Skill_W);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_E);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_E);
    STATEDELEGATE_EXIT(state, Yuki, Skill_E);

    return state;
}
FSMState* ER_ActionScript_Yuki::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Yuki, Skill_R);
    STATEDELEGATE_UPDATE(state, Yuki, Skill_R);
    STATEDELEGATE_EXIT(state, Yuki, Skill_R);

    return state;
}
