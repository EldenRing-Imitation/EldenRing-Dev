#include "pch.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine/CAnim3D.h>

ER_ActionScript_Hyunwoo::ER_ActionScript_Hyunwoo()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO)
    , m_pSounds{}
{
}

ER_ActionScript_Hyunwoo::~ER_ActionScript_Hyunwoo()
{
}


void ER_ActionScript_Hyunwoo::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Wait");
    SetStateGrade(eAccessGrade::BASIC);
    param.fData = 0.f;
}
void ER_ActionScript_Hyunwoo::WaitUpdate(tFSMData& param)
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
void ER_ActionScript_Hyunwoo::WaitExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Hyunwoo::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    bData[1]    : �̵��ӵ� ������ų �ۿ� ����
    fData		: ���� ���� �Ÿ� / �Ĺ� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ�� / 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */

    Animator3D()->SelectAnimation(L"Hyunwoo_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Hyunwoo::MoveUpdate(tFSMData& param)
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
void ER_ActionScript_Hyunwoo::MoveExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Hyunwoo::FarmingEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Wait");

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}

void ER_ActionScript_Hyunwoo::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Hyunwoo::RestEnter(tFSMData& param)
{
    /*
   iData[0] = �޽� �ִϸ��̼� ����Ǵ�
   fData    = ü������ð� ī��Ʈ
   */
    param.iData[0] = 0;
    param.fData = 0.f;
    Animator3D()->SelectAnimation(L"Hyunwoo_Rest_Start", false);
}
void ER_ActionScript_Hyunwoo::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Hyunwoo_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Hyunwoo_Rest_End", false);
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
void ER_ActionScript_Hyunwoo::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Arrive", false);
}
void ER_ActionScript_Hyunwoo::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Hyunwoo::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Hyunwoo_Death", false);
}
void ER_ActionScript_Hyunwoo::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Hyunwoo::DeadExit(tFSMData& param)
{
}


void ER_ActionScript_Hyunwoo::AttackEnter(tFSMData& param)
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

    // ���� ���۴ܰ� �ʱ�ȭ
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    if (param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Hyunwoo_Attack0", false);
        param.iData[0] = 6;
    }
    else
    {
        Animator3D()->SelectAnimation(L"Hyunwoo_Attack1", false);
        param.iData[0] = 6;
    }

    // Ÿ�ٹ������� ȸ��
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Hyunwoo::AttackUpdate(tFSMData& param)
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
void ER_ActionScript_Hyunwoo::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Hyunwoo::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Craft", false);
}

void ER_ActionScript_Hyunwoo::CraftUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::CraftExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Hyunwoo::Skill_W(tFSMData& _Data)
{
}

void ER_ActionScript_Hyunwoo::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E, eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::Skill_R(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_R, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_R, eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::Skill_QEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        float SpdValue = Skill->Float1();
        float Time = Skill->fActionTime;
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, Time, SpdValue);
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_SkillQ", false);
    }
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Hyunwoo_Wait", false);

    SetRotationToTarget(param.v4Data);
    SetStateGrade(eAccessGrade::UTMOST);

    SetRotationToTarget(param.v4Data);
}

void ER_ActionScript_Hyunwoo::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData[0] = 0;
    }

    if (curFrame > 10) {
        SetStateGrade(eAccessGrade::BASIC);
        param.iData[0] = 0;
    }
}

void ER_ActionScript_Hyunwoo::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WEnter(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WUpdate(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Hyunwoo_SkillE_Start", false);

    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 30.f;                                              // ��ų �Ÿ�
    float ClearDist = GetClearDistanceByWall(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // �̵� ���� �Ÿ�
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;   // ��ü �ִϸ��̼� ��� �ð�
    param.v4Data[3] = 0.f;                                              // �̵��� �Ÿ� �ʱ�ȭ.

    SetStateGrade(eAccessGrade::ADVANCED);
}

void ER_ActionScript_Hyunwoo::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    int curFrame = GetOwner()->Animator3D()->GetCurFrame();
    //������ �������� 31
    int EndFrame = GetOwner()->Animator3D()->GetCurAnim()->GetAnimClip().iEndFrame;

    // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
    if (param.v4Data[3] < param.v4Data[1] * 0.8)
    {
        // param.v4Data[0] : ��ų �Ÿ�(�ӵ�)
        // param.v4Data[1] : �̵� ���� �Ÿ�
        // param.v4Data[2] : ��ü �ִϸ��̼� ��� �ð�
        // param.v4Data[3] : �̵��� �Ÿ�

        Vec3 vPos = transform->GetRelativePos();
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        float speed = param.v4Data[0];

        if (param.v4Data[3] > (param.v4Data[1] / 5.f))
            speed = (float)(param.v4Data[0] * 1.5f);

        float CurFrameMoveDist = speed * param.v4Data[2] * DT;

        param.v4Data[3] += CurFrameMoveDist;

        vPos += vDir * CurFrameMoveDist;

        // ĳ���� �̵�
        transform->SetRelativePos(vPos);
    }
    else {
        //������ �ִϸ��̼� ��¾��ص� ��ǥ��ġ�� �����ϸ� ĵ�� ����
        SetStateGrade(eAccessGrade::BASIC);
        param.v4Data = Vec4();
        param.v4Data = Vec2();
        param.iData[0] = 0;
        ChangeState(ER_CHAR_ACT::WAIT);
    }

   if (GetOwner()->Animator3D()->IsFinish())
   {
       SetStateGrade(eAccessGrade::BASIC);
       // clear
       param.v4Data = Vec4();
       param.v4Data = Vec2();
   
       ChangeState(ER_CHAR_ACT::WAIT);
   }
}

void ER_ActionScript_Hyunwoo::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Hyunwoo::Skill_REnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Hyunwoo_SkillR_Start", false);

    SetRotationToTarget(param.v4Data);

    param.iData[0] = 0;

    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Hyunwoo::Skill_RUpdate(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    int curFrame = GetOwner()->Animator3D()->GetCurFrame();

    if (KEY_PRESSED(KEY::R)) {
        if (Animator->IsFinish()) {
            //Start�� ��������,
            if (param.iData[0] == 0) {
                Animator->SelectAnimation(L"Hyunwoo_SkillR_Loop", false);
            }
            else if (param.iData[0] == 1) {
                Animator->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            }
            ++param.iData[0];
        }
    }
    else {
        if (param.iData[0] != 2) {
            Animator->SelectAnimation(L"Hyunwoo_SkillR_End", false);
            param.iData[0] = 2;
        }
    }

    if (param.iData[0] == 2 && curFrame > 16) {
        SetStateGrade(eAccessGrade::BASIC);
    }

    if (param.iData[0] == 2 && Animator->IsFinish()) {
        SetStateGrade(eAccessGrade::BASIC);
        param.iData[0] = 0;

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Hyunwoo::Skill_RExit(tFSMData& param)
{
}




FSMState* ER_ActionScript_Hyunwoo::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Wait);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Wait);
    STATEDELEGATE_EXIT(state, Hyunwoo, Wait);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Move);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Move);
    STATEDELEGATE_EXIT(state, Hyunwoo, Move);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Farming);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Farming);
    STATEDELEGATE_EXIT(state, Hyunwoo, Farming);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Craft);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Craft);
    STATEDELEGATE_EXIT(state, Hyunwoo, Craft);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Rest);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Rest);
    STATEDELEGATE_EXIT(state, Hyunwoo, Rest);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Attack);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Attack);
    STATEDELEGATE_EXIT(state, Hyunwoo, Attack);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Arrive);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Arrive);
    STATEDELEGATE_EXIT(state, Hyunwoo, Arrive);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Dead);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Dead);
    STATEDELEGATE_EXIT(state, Hyunwoo, Dead);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_Q);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_Q);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_W);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_W);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_E);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_E);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Hyunwoo::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Hyunwoo, Skill_R);
    STATEDELEGATE_UPDATE(state, Hyunwoo, Skill_R);
    STATEDELEGATE_EXIT(state, Hyunwoo, Skill_R);

    return state;
}
