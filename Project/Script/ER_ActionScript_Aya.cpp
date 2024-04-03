#include "pch.h"
#include "ER_ActionScript_Aya.h"
#include "ER_DataScript_Character.h"

#include "ER_GameSystem.h"
#include "ER_ItemMgr.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"
#include "ER_PlayerScript.h"

#include <Engine/CAnim3D.h>

#include "ER_DataScript_Bullet.h"

ER_ActionScript_Aya::ER_ActionScript_Aya()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA)
{
}

ER_ActionScript_Aya::~ER_ActionScript_Aya()
{
}


void ER_ActionScript_Aya::WaitEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Idle", true);
    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
}
void ER_ActionScript_Aya::WaitUpdate(tFSMData& param)
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
void ER_ActionScript_Aya::WaitExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Aya::MoveEnter(tFSMData& param)
{
    /*
    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    bData[1]    : �̵��ӵ� ������ų �ۿ� ����
    fData		: ���� ���� �Ÿ� / �Ĺ� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ�� / 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */

    // ������� ��ų �����
    if (IsSkillOn(SKILLIDX::W_1))
    {
        // Move�� ���� �̵���ġ�� �������ش�.
        FindPath()->FindPath(param.v4Data);
        return;
    }

    Animator3D()->SelectAnimation(L"Aya_Run", true);
    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Aya::MoveUpdate(tFSMData& param)
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

    // �ִϸ��̼� ���� �Ǵ�

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
void ER_ActionScript_Aya::MoveExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::FarmingEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Idle", true);
    
    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    if (IsPlayer())
        ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Aya::FarmingExit(tFSMData& param)
{
    if (IsPlayer())
        ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Aya::RestEnter(tFSMData& param)
{
    /*
   iData[0] = �޽� �ִϸ��̼� ����Ǵ�
   fData    = ü������ð� ī��Ʈ
   */
    param.iData[0] = 0;
    param.fData[0] = 0.f;
    Animator3D()->SelectAnimation(L"Aya_Rest_Start", false);
}
void ER_ActionScript_Aya::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Aya_Rest_Loop");

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
            Animator3D()->SelectAnimation(L"Aya_Rest_End", false);
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
void ER_ActionScript_Aya::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Arrive", false);
}
void ER_ActionScript_Aya::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Aya::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_Death", false);
}
void ER_ActionScript_Aya::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Aya::DeadExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Craft", false);
    
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData[0] = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Aya::CraftUpdate(tFSMData& param)
{
    param.fData[0] += DT;

    if (param.iData[1] <= param.fData[0] || Animator3D()->IsFinish())
    {
        // ������ �����Լ�
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Aya::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData[0] = 0.f;
    STOPSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Aya::AttackEnter(tFSMData& param)
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

    Animator3D()->SelectAnimation(L"Aya_Attack", false);
    param.iData[0] = 5;

    // Ÿ�ٹ������� ȸ��
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::UTMOST);
}

void ER_ActionScript_Aya::AttackUpdate(tFSMData& param)
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

        // �Ѿ˾� �ݿ�����

        // ����ü ����

        ERCHARSOUND(NORMAL_ATTACK);

        CGameObject* Bullet = onew(CGameObject);
        ER_DataScript_Bullet* BulletScript = onew(ER_DataScript_Bullet);
        Bullet->AddComponent(BulletScript);

        BulletScript->init();
        BulletScript->SetForTarget(GetOwner(), 
            (CGameObject*)param.lParam, 
            GetProjSpawnPos(param.lParam), 
            30.f);
        BulletScript->Spawn();

        param.bData[1] = true;                          // Battle Event �Ϸ�
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

void ER_ActionScript_Aya::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Aya::Skill_Q(tFSMData& _Data)
{
    // ��ų ���������
    tFSMData PrevData = STATEDATA_GET(SKILL_Q);
    if (PrevData.bData[0] || 0 == _Data.lParam)
        return;

    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);

    if (IsInRange((CGameObject*)_Data.lParam, Skill->Range()))
    {
        if (Skill->Use(&GetStatus()->iSP))
        {
            STATEDATA_SET(SKILL_Q, _Data);
            ChangeState(ER_CHAR_ACT::SKILL_Q);
            SetStateGrade(eAccessGrade::UTMOST);
        }
    }
}

void ER_ActionScript_Aya::Skill_W(tFSMData& _Data)
{
    if (IsSkillOn(SKILLIDX::W_1))
        return;

    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);

    if (Skill->Use(&GetStatus()->iSP))
    {
        STATEDATA_SET(SKILL_W, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_W);
        SetStateGrade(eAccessGrade::UTMOST);
    }
}

void ER_ActionScript_Aya::Skill_E(tFSMData& _Data)
{
    tFSMData PrevData = STATEDATA_GET(SKILL_E);

    if (PrevData.bData[0])
        return;

    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);

    if (Skill->Use(&GetStatus()->iSP))
    {
        STATEDATA_SET(SKILL_E, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_E);
        SetStateGrade(eAccessGrade::UTMOST);
    }
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
    tFSMData PrevData = STATEDATA_GET(SKILL_R);

    if (PrevData.bData[0])
        return;

    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);

    if (Skill->Use(&GetStatus()->iSP))
    {
        STATEDATA_SET(SKILL_R, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_R);
        SetStateGrade(eAccessGrade::UTMOST);
    }
}

void ER_ActionScript_Aya::Skill_QEnter(tFSMData& param)
{
    // 7 , 16

    Animator3D()->SelectAnimation(L"Aya_SkillQ", false);
    SetRotationToTarget(param.v4Data);
    param.bData[0] = true;
    param.bData[1] = false;
    param.iData[0] = 7;
    param.bData[2] = false;
}

void ER_ActionScript_Aya::Skill_QUpdate(tFSMData& param)
{
    // ����/����� Ȯ��
    tStatus_Effect* statusefc = GetStatusEffect();
    float Atkspd = GetStatus()->fAttackSpeed;
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // �ִϸ��̼� �ӵ� ����
    Animator3D()->PlaySpeedValue(Atkspd);


    if (!param.bData[1] && param.iData[0] < Animator3D()->GetCurFrame())
    {
        // ĳ���� ���� ���� �˰���

        // ����ü ����
        CGameObject* Bullet = onew(CGameObject);
        ER_DataScript_Bullet* BulletScript = onew(ER_DataScript_Bullet);
        Bullet->AddComponent(BulletScript);

        BulletScript->init();
        BulletScript->SetForTarget(
            GetOwner(),
            (CGameObject*)param.lParam,
            GetProjSpawnPos(param.lParam),
            30.f);

        // 2Ÿ
        if (param.bData[2])
        {
            ERCHARSOUND(SKILLQ);
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            BulletScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Aya::SkillQ2, skill, 1);
        }
        // 1Ÿ
        else
        {
            ERCHARSOUND(NORMAL_ATTACK);
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            BulletScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Aya::SkillQ1, skill, 0);
        }

        BulletScript->Spawn();

        param.bData[1] = true;                          // Battle Event �Ϸ�


        /// �ƾ� 2Ÿ ��ų���ݺ�ȯ
        if (param.iData[0] < 16)
        {
            param.iData[0] = 16;
            param.bData[1] = false;
            param.bData[2] = true;
        }
    }

    if (Animator3D()->IsFinish())
    {
        param.bData[0] = false;

        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::Skill_QExit(tFSMData& param)
{
    param.bData[0] = false;
}

void ER_ActionScript_Aya::Skill_WEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_SkillW_Shot", false);

    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;
    param.v2Data.Normalize();

    param.v4Data.x = vPos.x;
    param.v4Data.y = vPos.y;
    param.v4Data.z = vPos.z;

    param.iData[0] = 0;
    param.fData[0] = 0.f;
    param.fData[1] = 0.f;
    param.fData[2] = 0.f;

    param.bData[0] = false;
}

void ER_ActionScript_Aya::Skill_WUpdate(tFSMData& param)
{
    param.fData[1] += DT;
    param.fData[2] += DT;

    // �Ѿ� �߻�, 2�ʰ� 6~ 8��
    if (0.3f < param.fData[2])
    {
        CGameObject* Bullet = onew(CGameObject);
        ER_DataScript_Bullet* BulletScript = onew(ER_DataScript_Bullet);
        Bullet->AddComponent(BulletScript);

        // �ʱ� ��ų���� �������� �߻�
        Vec3 vPos = Transform()->GetRelativePos();

        Vec3 vDir = { param.v2Data.x, 0.f, param.v2Data.y };

        vPos.y += 1.3f;
        vPos += vDir * 0.7f;

        BulletScript->init();
        BulletScript->SetForDir(
            GetOwner(),
            vPos,
            30.f,
            1.f);

        ERCHARSOUND(SKILLW);
        tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
        BulletScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Aya::SkillW, skill, 0);

        BulletScript->Spawn();

        param.fData[2] = 0.f;
    }

    // ���� ����
    Vec3 vPos = Transform()->GetRelativePos();
    Vec2 vDir;//�̵��� �̵��ϴ� ���� ����.
    Vec3 vDestPoint; //�̵��� �������� ��ġ
    
    if (KEY_TAP(KEY::RBTN)) {
        vDestPoint = GetFocusPoint();
        Vec3 tempDir = vDestPoint - vPos;
        vDir.x = tempDir.x;
        vDir.y = tempDir.z;
        vDir.Normalize();

        float angleRad = atan2(vDir.y, vDir.x) - atan2(param.v2Data.y, param.v2Data.x);
        // ������ ���� ��ȯ
        float angleDeg = (float)(angleRad * (180.0 / XM_PI));

        // ������ -180 ~ 180 ������ ����
        if (angleDeg > 180.0) {
            angleDeg -= 360.0;
        }
        else if (angleDeg < -180.0) {
            angleDeg += 360.0;
        }

        //param.v4Data���� ���� �̵��ϴ� �������� ��.
        param.v4Data.x = vDestPoint.x;
        param.v4Data.y = vDestPoint.y;
        param.v4Data.z = vDestPoint.z;

        //angleDeg���� ������ ��.
        param.fData[0] = angleDeg;

        param.bData[0] = true;

        GetOwner()->FindPath()->FindPath(param.v4Data);
    }

    //�ִϸ��̼��� ���������� �̵� �ִϸ��̼� ����
    if ((abs(vPos.x- param.v4Data.x)>0.1f || abs(vPos.z - param.v4Data.z)>0.1f) && param.bData[0]) {
        // ���� ����
        if (param.fData[0] > -45 && param.fData[0] <= 45) {
            if (param.iData[0] != 1) {
                Animator3D()->SelectAnimation(L"Aya_SkillW_Forward", true);
                param.iData[0] = 1;
            }
        }
        else if (param.fData[0] > 45 && param.fData[0] <= 135) {
            if (param.iData[0] != 2) {
                Animator3D()->SelectAnimation(L"Aya_SkillW_Left", true);
                param.iData[0] = 2;
            }
        }
        else if (param.fData[0] > -135 && param.fData[0] <= -45) {
            if (param.iData[0] != 3) {
                Animator3D()->SelectAnimation(L"Aya_SkillW_Right", true);
                param.iData[0] = 3;
            }
        }
        else {
            if (param.iData[0] != 4) {
                Animator3D()->SelectAnimation(L"Aya_SkillW_Back", true);
                param.iData[0] = 4;
            }
        }
        param.bData[0] = false;
    }
    
    //������ �׳� ��.
    else if((abs(vPos.x - param.v4Data.x) < 0.1f && abs(vPos.z - param.v4Data.z) < 0.1f)){
        Animator3D()->SelectAnimation(L"Aya_SkillW_Shot", false);
        param.iData[0] = 0;
    }

    //������ ��ġ�� ���� ��ġ�� ���� ���� ��, ��� �̵���.
    if (vPos != param.v4Data) {
        float speed = m_Data->GetStatus()->fMovementSpeed;
        GetOwner()->FindPath()->PathMove(speed, false);
    }

    if (param.fData[1] > 2.f) {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData[0] = 0;
        param.v2Data = Vec2();
        param.v4Data = Vec4();
        param.fData[0] = 0.f;
        param.fData[1] = 0.f;
        param.fData[2] = 0.f;
    }
}

void ER_ActionScript_Aya::Skill_WExit(tFSMData& param)
{
    param.iData[0] = 0;
    param.v2Data = Vec2();
    param.v4Data = Vec4();
    param.fData[0] = 0.f;
    param.fData[1] = 0.f;
    param.fData[2] = 0.f;
}

void ER_ActionScript_Aya::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Aya_SkillE", false);

    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.fData[0] = 5.f;                                                       // ��ų �Ÿ�
    float ClearDist = GetClearDistance(vDir, param.fData[0]);
    param.fData[1] = ClearDist;                                                 // �̵� ���� �Ÿ�
    param.fData[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;     // ��ü �ִϸ��̼� ��� �ð�
    param.fData[3] = 0.f;                                                       // �̵��� �Ÿ� �ʱ�ȭ.

    param.bData[0] = true;

    // Q,W ��ų ��Ÿ���� 20%�� �ǵ����޴´�
    tSkill_Info* SkillQ = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    tSkill_Info* SkillW = m_Data->GetSkill((UINT)SKILLIDX::W_1);

    if (!SkillQ->IsUsable)
    {
        float QCoolRatio = SkillQ->MaxCooldown() * 0.2f;
        SkillQ->fCoolDown = SkillQ->fCoolDown - QCoolRatio <= 0.5f ? 0.5f : SkillQ->fCoolDown - QCoolRatio;
    }
    if (!SkillW->IsUsable)
    {
        float WCoolRatio = SkillW->MaxCooldown() * 0.2f;
        SkillW->fCoolDown = SkillW->fCoolDown - WCoolRatio <= 0.5f ? 0.5f : SkillW->fCoolDown - WCoolRatio;
    }

    // �����Ÿ�(20%) �ڷ���Ʈ����
    Vec3 vMoveDir(param.v2Data.x, 0.f, param.v2Data.y);
    
    float fMoveDist = param.fData[1] * 0.2f;
    param.fData[3] += fMoveDist;
    vPos += vMoveDir * fMoveDist;

    // ĳ���� �̵�
    Transform()->SetRelativePos(vPos);

    SetStateGrade(eAccessGrade::ADVANCED);

    ERCHARSOUND(SKILLE);
}

void ER_ActionScript_Aya::Skill_EUpdate(tFSMData& param)
{
    Animator3D()->PlaySpeedValue(1.5f);

    // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
    if (param.fData[3] < param.fData[1])
    {
        // param.fData[0] : ��ų �Ÿ�(�ӵ�)
        // param.fData[1] : �̵� ���� �Ÿ�
        // param.fData[2] : ��ü �ִϸ��̼� ��� �ð�
        // param.fData[3] : �̵��� �Ÿ�

        Vec3 vPos = Transform()->GetRelativePos();
        Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);

        float speed = param.fData[0];

        if (param.fData[3] > (param.fData[1] / 5.f))
            speed = param.fData[0] * 2.f;

        float CurFrameMoveDist = speed * param.fData[2] * DT;

        param.fData[3] += CurFrameMoveDist;

        vPos += vDir * CurFrameMoveDist;

        // ĳ���� �̵�
        Transform()->SetRelativePos(vPos);
    }

    if (Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        // clear
        param.fData[0] = 0.f;
        param.fData[1] = 0.f;
        param.fData[2] = 0.f;
        param.fData[3] = 0.f;
        param.v2Data = Vec2();
        param.iData[0] = 0;

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::Skill_EExit(tFSMData& param)
{
    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
    param.fData[1] = 0.f;
    param.fData[2] = 0.f;
    param.fData[3] = 0.f;
    param.v2Data = Vec2();
    param.iData[0] = 0;

    param.bData[0] = false;
}

void ER_ActionScript_Aya::Skill_REnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Aya_SkillR_Start", false);
    param.bData[0] = true;
    
    param.bData[1] = false;
    param.bData[2] = false;
    param.bData[3] = false; // ���� ��� ��ũ

    ERCHARSOUND(SKILLR_READY);
    ERCHARSOUND(SKILLR_CHAR);
}

void ER_ActionScript_Aya::Skill_RUpdate(tFSMData& param)
{
    if (!param.bData[3] && 46 < Animator3D()->GetCurFrame())
    {
        ERCHARSOUND(SKILLR_ACTION);
        param.bData[3] = true;
    }

    if (Animator3D()->IsFinish())
    {
        if (param.bData[1])
        {
            // ��ų ����
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
        else
        {
            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            Vec3 vPos = Transform()->GetRelativePos();

            for (auto Target : vecChar)
            {
                // ���� �˻�
                if (Target == GetOwner())
                    continue;

                // �Ÿ� �˻�
                Vec3 TargetPos = Target->Transform()->GetRelativePos();
                float dist = Vec3::Distance(vPos, TargetPos);

                if (8.f < dist)
                    continue;

                // ����ȿ�� 

                // ��ų���� ������
                const tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
                BATTLE_SKILL(GetOwner(), Target, ER_ActionScript_Aya, SkillR, skill, 0);
            }

            Animator3D()->SelectAnimation(L"Aya_SkillR_End", false);
            param.bData[1] = true;
        }
    }
}

void ER_ActionScript_Aya::Skill_RExit(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[1] = false;
}


FSMState* ER_ActionScript_Aya::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Wait);
    STATEDELEGATE_UPDATE(state, Aya, Wait);
    STATEDELEGATE_EXIT(state, Aya, Wait);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Move);
    STATEDELEGATE_UPDATE(state, Aya, Move);
    STATEDELEGATE_EXIT(state, Aya, Move);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Farming);
    STATEDELEGATE_UPDATE(state, Aya, Farming);
    STATEDELEGATE_EXIT(state, Aya, Farming);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateCraft()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Craft);
    STATEDELEGATE_UPDATE(state, Aya, Craft);
    STATEDELEGATE_EXIT(state, Aya, Craft);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Rest);
    STATEDELEGATE_UPDATE(state, Aya, Rest);
    STATEDELEGATE_EXIT(state, Aya, Rest);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateAttack()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Attack);
    STATEDELEGATE_UPDATE(state, Aya, Attack);
    STATEDELEGATE_EXIT(state, Aya, Attack);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Arrive);
    STATEDELEGATE_UPDATE(state, Aya, Arrive);
    STATEDELEGATE_EXIT(state, Aya, Arrive);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateDead()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Dead);
    STATEDELEGATE_UPDATE(state, Aya, Dead);
    STATEDELEGATE_EXIT(state, Aya, Dead);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_Q()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_Q);
    STATEDELEGATE_UPDATE(state, Aya, Skill_Q);
    STATEDELEGATE_EXIT(state, Aya, Skill_Q);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_W()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_W);
    STATEDELEGATE_UPDATE(state, Aya, Skill_W);
    STATEDELEGATE_EXIT(state, Aya, Skill_W);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_E()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_E);
    STATEDELEGATE_UPDATE(state, Aya, Skill_E);
    STATEDELEGATE_EXIT(state, Aya, Skill_E);

    return state;
}

FSMState* ER_ActionScript_Aya::CreateSkill_R()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Aya, Skill_R);
    STATEDELEGATE_UPDATE(state, Aya, Skill_R);
    STATEDELEGATE_EXIT(state, Aya, Skill_R);

    return state;
}


int ER_ActionScript_Aya::SkillQ1()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(GetStatus()->iAttackPower * 1.f);

    return Dmg;
}

int ER_ActionScript_Aya::SkillQ2()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.3f);

    return Dmg;
}

int ER_ActionScript_Aya::SkillW()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.5f);

    return Dmg;
}

int ER_ActionScript_Aya::SkillR()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.2f);
    Dmg += Dmg * GetStatus()->iSkillAmplification;

    return Dmg;
}
