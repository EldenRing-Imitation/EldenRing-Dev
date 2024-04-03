#include "pch.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_DataScript_Character.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include "ER_ItemMgr.h"
#include "ER_GameSystem.h"

#include <Engine\CAnim3D.h>

//����Ʈ ����
#include "ER_YukiBAEffect.h"
#include "ER_YukiQEffect.h"
#include "ER_YukiWEffect.h"
#include "ER_YukiEEffect.h"
#include "ER_YukiREffect.h"

ER_ActionScript_Yuki::ER_ActionScript_Yuki()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI)
{
}

ER_ActionScript_Yuki::~ER_ActionScript_Yuki()
{
}

void ER_ActionScript_Yuki::begin()
{
    ER_ActionScript_Character::begin();
    ER_YukiBAEffect* BAEffect = onew(ER_YukiBAEffect);
    ER_YukiQEffect* QEffect = onew(ER_YukiQEffect);
    ER_YukiWEffect* WEffect = onew(ER_YukiWEffect);
    ER_YukiEEffect* EEffect = onew(ER_YukiEEffect);
    ER_YukiREffect* REffect = onew(ER_YukiREffect);

    GetOwner()->AddComponent(BAEffect);
    GetOwner()->AddComponent(QEffect);
    GetOwner()->AddComponent(WEffect);
    GetOwner()->AddComponent(EEffect);
    GetOwner()->AddComponent(REffect);
}

void ER_ActionScript_Yuki::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Wait", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
}
void ER_ActionScript_Yuki::WaitUpdate(tFSMData& param)
{
    // 0.5�ʸ��� ü��ȸ��
    param.fData[0] += DT;

    if (0.5f <= param.fData[0])
    {
        // HP/SP �ڿ� ȸ��
        m_Data->HPRegen();
        m_Data->SPRegen();

        // ü����� ī��Ʈ �ʱ�ȭ
        param.fData[0] -= 0.5f;
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
        if (IsInRange((CGameObject*)param.lParam, param.fData[0]))
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

    // ����/����� ȿ�� �ݿ�
    tStatus_Effect* SpeedEfc = m_Data->GetStatusEffect();

    // �ִϸ��̼� �ݿ�
    float SpdEfcAnim = ((SpeedEfc->GetIncSPD()) * 10.f) + ((SpeedEfc->GetDecSPD()) * -10.f);
    Animator3D()->PlaySpeedValue(SpdEfcAnim);

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
    if(IsPlayer())
        ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Yuki::FarmingExit(tFSMData& param)
{
    if(IsPlayer())
        ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Yuki::RestEnter(tFSMData& param)
{
    /*
    iData[0] = �޽� �ִϸ��̼� ����Ǵ�
    fData    = ü������ð� ī��Ʈ
    */
    param.iData[0] = 0;
    param.fData[0] = 0.f;
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
        param.fData[0] += DT;

        if (0.5f <= param.fData[0])
        {
            // HP/SP �ڿ� ȸ�� 5�� ������ ȸ��
            m_Data->HPRegen(5.f);
            m_Data->SPRegen(5.f);

            // �ڿ���� ī��Ʈ �ʱ�ȭ
            param.fData[0] -= 0.5f;
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

void ER_ActionScript_Yuki::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Yuki_Craft", false);
    
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData[0] = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Yuki::CraftUpdate(tFSMData& param)
{
    param.fData[0] += DT;

    if (param.iData[1] <= param.fData[0] || Animator3D()->IsFinish())
    {
        // ������ �����Լ�
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData[0] = 0.f;
    STOPSOUND(CRAFT_SOUND);
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
   iData[1]     : ���ݸ�� ����
   lParam		: Ÿ�� ������Ʈ
   RParam		: Ÿ�� ���� ������Ʈ
   */

   // ���� ���۴ܰ� �ʱ�ȭ
    param.bData[0] = true;
    param.bData[1] = false;
    param.bData[3] = !param.bData[3];

    // Q��ų ���� ���� : ��ȭ��Ÿ
    if (IsSkillOn(SKILLIDX::Q_1))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillQ", false);
        m_Data->GetSkill((UINT)SKILLIDX::Q_1)->ActionOver();
        param.iData[0] = 7;
        param.iData[1] = 2;
        ERCHARSOUND(SKILLQ_ATTACK);
    }
    else if (!param.bData[3])
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack0", false);
        param.iData[0] = 8;
        param.iData[1] = 0;
        ERCHARSOUND(ATTACK_NORMAL1);
    }
    else
    {
        Animator3D()->SelectAnimation(L"Yuki_Attack1", false);
        param.iData[0] = 8;
        param.iData[1] = 1;
        ERCHARSOUND(ATTACK_NORMAL2);
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
     bData[3]   : ���ݸ�� ����
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
        switch (param.iData[1])
        {
        case 0:
        {

        }
        case 1:
        {
            // �⺻ ����
            BATTLE_COMMON(GetOwner(), param.lParam);
            ERCHARSOUND(HITSOUND);
            break;
        }
        case 2:
        {
            // ��ȭ��Ÿ
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            BATTLE_SKILL(GetOwner(), (CGameObject*)param.lParam, ER_ActionScript_Yuki, SkillQ, skill, 0);
            ERCHARSOUND(SKILLQ_HIT1);
            break;
        }
        }

        Vec3 _dir = GetProjSpawnPos(param.lParam) - GetOwner()->Transform()->GetRelativePos();
        _dir.y = 0.f;
        _dir.Normalize();
        Vec3 resultPos = GetProjSpawnPos(param.lParam) - _dir * 0.4f;
        resultPos.y += 0.2f;
        if (param.iData[1] == 0) {
            GetOwner()->GetScript<ER_YukiBAEffect>()->SpawnSecondEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }
        else if (param.iData[1] == 1) {            
            GetOwner()->GetScript<ER_YukiBAEffect>()->SpawnFirstEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }
        else if (param.iData[1] == 2) {
            GetOwner()->GetScript<ER_YukiQEffect>()->SpawnAttackEffect(resultPos, GetOwner()->Transform()->GetRelativeRot());
        }

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

void ER_ActionScript_Yuki::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Yuki::Skill_W(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_W);
    if (Prevdata.bData[0])
        return;

    STATEDATA_SET(SKILL_W, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Yuki::Skill_E(tFSMData& _Data)
{
    tFSMData Prevdata = STATEDATA_GET(SKILL_E);
    if (Prevdata.bData[0])
        return;

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
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        // ����Ʈ ���
        ERCHARSOUND(SKILLQ_MOTION);

        Vec3 resultPos = Transform()->GetRelativePos();
        GetOwner()->GetScript<ER_YukiQEffect>()->SpawnEffect(resultPos, Transform()->GetRelativeRot());
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QUpdate(tFSMData& param)
{
    // ���� ������ ���½�ų.

    SetStateGrade(eAccessGrade::BASIC);

    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else if ((UINT)ER_CHAR_ACT::ATTACK == m_iPrevState)
        ChangeState(ER_CHAR_ACT::ATTACK);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Yuki::Skill_WEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    
    if (Skill->Use(&GetStatus()->iSP, true))
    {
        param.bData[0] = true;

        // ����ġ�� ��ٿ� 3�� ���� int2
        tSkill_Info* SkillE = m_Data->GetSkill((UINT)SKILLIDX::E_1);
        if (!SkillE->IsUsable)
        {
            float QCoolRatio = (float)Skill->Int2();
            SkillE->fCoolDown = SkillE->fCoolDown - QCoolRatio <= 0.5f ? 0.5f : SkillE->fCoolDown - QCoolRatio;
        }

        // [ ���� ]
        // ���ݷ� ���� int1
        // ���� 30% ���� float1,
        int AtkValue = Skill->Int1();
        int DefValue = (int)((GetStatus()->iDefense * Skill->Float1()));
        float ActionTime = Skill->ActionTime();

        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_ATK, ActionTime, (float)AtkValue);
        m_Data->GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_DEF, ActionTime, (float)DefValue);

        ERCHARSOUND(SKILLW_MOTION);

        GetOwner()->GetScript<ER_YukiWEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());

        Animator3D()->SelectAnimation(L"Yuki_SkillW_Upper_Wait", false);
        SetStateGrade(eAccessGrade::UTMOST);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);

}

void ER_ActionScript_Yuki::Skill_WUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        param.bData[0] = false;
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Yuki::Skill_WExit(tFSMData& param)
{
    param.bData[0] = false;
}

void ER_ActionScript_Yuki::Skill_EEnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    
    if (Skill->Use(&GetStatus()->iSP))
    {
        Animator3D()->SelectAnimation(L"Yuki_SkillE_Move", false);
        SetRotationToTarget(param.v4Data);

        // ��ų ���� ����
        param.bData[0] = true;

        // ���� ����
        param.bData[1] = false;

        // ���� ����
        Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
        Vec3 vDir = (param.v4Data - vPos).Normalize();

        // �浹 ���� ����
        param.iData[0] = 0;

        // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
        param.v2Data.x = vDir.x;
        param.v2Data.y = vDir.z;

        param.fData[0] = 6.f;                                                          // ��ų �Ÿ�
        float ClearDist = GetClearDistance(vDir, param.fData[0]);
        param.fData[1] = ClearDist;                                                    // �̵� ���� �Ÿ�
        param.fData[2] = (float)Animator3D()->GetCurAnim()->GetAnimClip().dEndTime;    // ��ü �ִϸ��̼� ��� �ð�
        param.fData[3] = 0.f;                                                          // �̵��� �Ÿ� �ʱ�ȭ.

        SetStateGrade(eAccessGrade::UTMOST);

        ERCHARSOUND(SKILLE_MOTION);
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_EUpdate(tFSMData& param)
{
    // Animator3D()->PlaySpeedValue(1.5f);

    switch (param.iData[0])
    {
    case 0:
    {
        // 4frm ���ĺ��� �̵�
        if (4 < Animator3D()->GetCurFrame())
        {
            param.iData[0] = 1;
            return;
        }

        break;
    }
    case 1:
    {
        // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
        if (param.fData[3] < param.fData[1] - 1.f)
        {
            // param.fData[0] : ��ų �Ÿ�(�ӵ�)
            // param.fData[1] : �̵� ���� �Ÿ�
            // param.fData[2] : ��ü �ִϸ��̼� ��� �ð�
            // param.fData[3] : �̵��� �Ÿ�

            Vec3 vPos = Transform()->GetRelativePos();
            Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

            float speed = param.fData[0];

            if (param.fData[3] > (param.fData[1] / 5.f))
                speed = param.fData[0] * 1.2f;

            float CurFrameMoveDist = speed * param.fData[0] * DT;

            param.fData[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            Transform()->SetRelativePos(vPos);
        }

        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    case 2:
    {
        if (!param.bData[1])
        {
            // ��ų Ÿ��
            STOPSOUND(SKILLE_MOTION);
            ERCHARSOUND(SKILLE_ATTACK);
            ERCHARSOUND(SKILLE_HIT1);

            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
            BATTLE_SKILL(GetOwner(), (CGameObject*)param.lParam, ER_ActionScript_Yuki, SkillE, skill, 0);

            param.bData[1] = true;

            GetOwner()->GetScript<ER_YukiEEffect>()->SpawnEffect(Transform()->GetRelativePos(), Transform()->GetRelativeRot());
        }

        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_EExit(tFSMData& param)
{
    // ��ų ���� ����
    param.bData[0] = false;
    param.bData[1] = false;
    // �浹 ���� ����
    param.iData[0] = 0;
    param.v2Data = Vec2();

    param.fData[0] = 0.f;
    param.fData[1] = 0.f;
    param.fData[2] = 0.f;
    param.fData[3] = 0.f;
}

void ER_ActionScript_Yuki::Skill_REnter(tFSMData& param)
{
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

    // ��ų�� ����� �� �ִ��� ���� �Ǵ�
    if (Skill->IsUsable && Skill->UseSP() <= GetStatus()->iSP)
    {
        GetOwner()->Animator3D()->SelectAnimation(L"Yuki_Wait", true);

        param.bData[0] = false;
        param.bData[1] = false;
        param.iData[0] = 0;     // 0. �⺻, 1. ��ų ����, 2. ��ų ��¡ ����, 3. ��ų ǥ�� ����
        param.iData[1] = 31;    // End Anim Hit Frame
    }
    else if((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else if ((UINT)ER_CHAR_ACT::ATTACK == m_iPrevState)
        ChangeState(ER_CHAR_ACT::ATTACK);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Yuki::Skill_RUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0:
    {
        // ��ų ����
        Vec3 vTargetPoint = GetFocusPoint();
        SetRotationToTarget(vTargetPoint);

        if (KEY_TAP(KEY::LBTN) || KEY_TAP(KEY::R))
        {
            tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            if (Skill->Use(&GetStatus()->iSP))
            {
                param.bData[0] = true;
                param.bData[1] = false;
                param.iData[0] = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ��¡ ����, 3. ��ų ǥ�� ����
                param.iData[1] = 31;    // End Anim Hit Frame

                GetOwner()->GetScript<ER_YukiREffect>()->AreaSpawn(Transform()->GetRelativePos(), Transform()->GetRelativeRot());

                // ��ų �ߵ�
                GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_Loop", false);
                SetStateGrade(eAccessGrade::UTMOST);
                ERCHARSOUND(SKILLR_ACTIVE);
            }
            else
            {
                ChangeState(ER_CHAR_ACT::WAIT);
            }
        }
        else if (KEY_TAP(KEY::ESC))
        {
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        break;
    }
    case 1:
    {
        // ��ų Ÿ�ݹ��� �ִϸ��̼� ���
        // Ư�� �����ӿ� ���� ��ǰ� �Բ� �����Ŵ���ȣ��


        if (GetOwner()->Animator3D()->IsFinish())
        {
            ERCHARSOUND(SKILLR_ATTACK);
            // ������ ó��
            
            bool IsHit = false;

            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            for (auto Target : vecChar)
            {
                // ���� �˻�
                if (Target == GetOwner())
                    continue;

                // �ݿ����� ��ų���� �Ǵ�
                if (!IsInRangeWithAngle(GetOwner(), Target, 5.f, 90.f))
                    continue;

                float DebufTime = 1.4f;
                float SpeedValue = 0.9f;

                // float 1 ����
                // float 2 �ִ뵩����

                // �̵��ӵ� 90% �����
                Target->GetScript<ER_DataScript_Character>()->GetStatusEffect()->
                    ActiveEffect((UINT)eStatus_Effect::DECREASE_SPD, DebufTime, SpeedValue);

                // ������ ����
                IsHit = true;
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Yuki, SkillR1, skill, 0);
            }

            if(IsHit)
                ERCHARSOUND(SKILLR_HIT);

            GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillR_End", false);
            param.iData[0] = 2;
            
        }
        break;
    }
    case 2:
    {
        if (!param.bData[1] && param.iData[1] < Animator3D()->GetCurFrame())
        {
            bool IsHit = false;

            param.bData[1] = true;

            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

            for (auto Target : vecChar)
            {
                // ���� �˻�
                if (Target == GetOwner())
                    continue;

                // �ݿ����� ��ų���� �Ǵ�
                if (!IsInRangeWithAngle(GetOwner(), Target, 5.f, 90.f))
                    continue;

                // ��ų�������� Ÿ���� �ִ�ü���� ������� lParam ������ ����
                param.lParam = (DWORD_PTR)Target;

                // ������ ����
                IsHit = true;
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Yuki, SkillR2, skill, 1);
            }

            if (IsHit)
            {
                ERCHARSOUND(SKILLR_END);
                ERCHARSOUND(SKILLR_HIT);
            }
        }

        if (GetOwner()->Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }

        break;
    }
    }
}

void ER_ActionScript_Yuki::Skill_RExit(tFSMData& param)
{
    // ��ų���� ������ ����
    param.iData[0] = 0;
    param.bData[0] = false;
    param.bData[1] = false;
}


int ER_ActionScript_Yuki::SkillQ()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 1.f);

    return Dmg;
}

int ER_ActionScript_Yuki::SkillE()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.65f);

    return Dmg;
}

int ER_ActionScript_Yuki::SkillR1()
{
    // float 1 ����
    // float 2 �ִ뵩����


    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * skill->Float1());

    return Dmg;
}

int ER_ActionScript_Yuki::SkillR2()
{
    tFSMData StateData = STATEDATA_GET(SKILL_R);
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int TargetMaxHP = ((CGameObject*)StateData.lParam)->GetScript<ER_DataScript_Character>()->GetStatus()->iMaxHP;
    
    // �ִ� HP ��ʵ�����
    int Dmg = (int)(TargetMaxHP * skill->Float2());

    return Dmg;
}

void ER_ActionScript_Yuki::BeginOverlap(CCollider3D* _Other)
{
    // �浹üũ true, �����Ÿ� �������̵�,
    // Move Attack ���
    tFSMData SkillE = STATEDATA_GET(SKILL_E);
    CGameObject* Target = _Other->GetOwner();

    // SkillE �������̰� �̵������϶�
    if (SkillE.bData[0]  && 1 == SkillE.iData[0])
    {
        if (!IsCharacter(Target) || IsDead(Target))
            return;

        // param.fData[0] : ��ų �Ÿ�(�ӵ�)
        // param.fData[1] : �̵� ���� �Ÿ�
        // param.fData[2] : ��ü �ִϸ��̼� ��� �ð�
        // param.fData[3] : �̵��� �Ÿ�

        // �̶� ù �浹�� ������Ʈ�� Ÿ������ ����
        SkillE.lParam = (DWORD_PTR)Target;

        GetOwner()->Animator3D()->SelectAnimation(L"Yuki_SkillE_Attack", false);
        SkillE.iData[0] = 2;
        
        // 1.f �߰� ���� �̵�, �ִ��̵����ɰŸ� ����
        Vec3 vPos = Target->Transform()->GetRelativePos();
        Vec3 vDir(SkillE.v2Data.x, 0.f, SkillE.v2Data.y);

        // �����̵��Ÿ� �ִ��̵��Ÿ������ʵ��� ����
        float ClearDist = SkillE.fData[3] + 1.f <= SkillE.fData[1] ? 1.f : SkillE.fData[1] - SkillE.fData[3];

        vPos += vDir * ClearDist;

        Transform()->SetRelativePos(vPos);

        STATEDATA_SET(SKILL_E, SkillE);
        return;
    }

}

void ER_ActionScript_Yuki::OnOverlap(CCollider3D* _Other)
{
}

void ER_ActionScript_Yuki::EndOverlap(CCollider3D* _Other)
{
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
