#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"

#include "ER_GameSystem.h"
#include "ER_ItemMgr.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"
#include "ER_PlayerScript.h"

#include <Engine\CAnim3D.h>

#include "ER_DataScript_Arrow.h"

ER_ActionScript_Rio::ER_ActionScript_Rio()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
    , m_pSounds{}
    , m_BowType(false)
{
}

ER_ActionScript_Rio::~ER_ActionScript_Rio()
{
}

void ER_ActionScript_Rio::WaitEnter(tFSMData& param)
{
    /*
    fData   : ����ð� ī��Ʈ
    */
    if (m_BowType)
        Animator3D()->SelectAnimation(L"Rio_Long_Wait", true);
    else
        Animator3D()->SelectAnimation(L"Rio_Short_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);
    param.fData[0] = 0.f;
}
void ER_ActionScript_Rio::WaitUpdate(tFSMData& param)
{
    // 0.5�ʸ��� ȸ��
    param.fData[0] += DT;
    
    if (0.5f <= param.fData[0])
    {
        // HP/SP �ڿ� ȸ��
        m_Data->HPRegen();
        m_Data->SPRegen();

        // �ڿ���� ī��Ʈ �ʱ�ȭ
        param.fData[0] -= 0.5f;
    }
}
void ER_ActionScript_Rio::WaitExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Rio::MoveEnter(tFSMData& param)
{
    if (m_BowType)
        Animator3D()->SelectAnimation(L"Rio_Long_Run", true);
    else
        Animator3D()->SelectAnimation(L"Rio_Short_Run", true);

    FindPath()->FindPath(param.v4Data);
}
void ER_ActionScript_Rio::MoveUpdate(tFSMData& param)
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
    
    // �̵��ӵ� ����
    float fMoveSpeed = GetStatus()->fMovementSpeed;
    fMoveSpeed += (fMoveSpeed * SpeedEfc->GetIncSPD()) - (fMoveSpeed * SpeedEfc->GetDecSPD());

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!FindPath()->PathMove(fMoveSpeed))
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Rio::MoveExit(tFSMData& param)
{
    // ��� ����
}

void ER_ActionScript_Rio::RestEnter(tFSMData& param)
{
    /*
    iData[0] = �޽� �ִϸ��̼� ����Ǵ�
    fData    = ü������ð� ī��Ʈ
    */
    param.iData[0] = 0;
    param.fData[0] = 0.f;
    Animator3D()->SelectAnimation(L"Rio_Rest_Start", false);
}
void ER_ActionScript_Rio::RestUpdate(tFSMData& param)
{
    switch (param.iData[0])
    {
    case 0: // ���� ����
    {
        // �ִϸ��̼� ���̸�ŭ ���������� UI ���

        if (Animator3D()->IsFinish())
        {
            Animator3D()->SelectAnimation(L"Rio_Rest_Loop");
            
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
            Animator3D()->SelectAnimation(L"Rio_Rest_End", false);
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
void ER_ActionScript_Rio::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::ArriveEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Rio_Arrive", false);
}
void ER_ActionScript_Rio::ArriveUpdate(tFSMData& param)
{
    if (Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Rio::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::DeadEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Rio_Death", false);
}
void ER_ActionScript_Rio::DeadUpdate(tFSMData& param)
{
}
void ER_ActionScript_Rio::DeadExit(tFSMData& param)
{
}


void ER_ActionScript_Rio::FarmingEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Wait", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Wait", true);

    SetStateGrade(eAccessGrade::BASIC);

    CGameObject* ItemObj = ((CGameObject*)param.lParam);

    ER_DataScript_ItemBox* ItemBox = ItemObj->GetScript<ER_DataScript_ItemBox>();
    ER_UIMgr::GetInst()->OpenItemBoxUI(ItemBox);
}
void ER_ActionScript_Rio::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->CloseItemBoxUI();
}

void ER_ActionScript_Rio::CraftEnter(tFSMData& param)
{
    Animator3D()->SelectAnimation(L"Rio_CraftMetal", false);
    
    int ItemGrade = ER_ItemMgr::GetInst()->GetItemObj(param.iData[0])->GetScript<ER_DataScript_Item>()->GetGrade();
    int CraftTime = 2 + (2 * ItemGrade);
    param.bData[0] = true;
    param.iData[1] = (int)CraftTime;
    param.fData[0] = 0.f;

    ERCHARSOUND(CRAFT_SOUND);
}
void ER_ActionScript_Rio::CraftUpdate(tFSMData& param)
{
    param.fData[0] += DT;

    if (param.iData[1] <= param.fData[0] || Animator3D()->IsFinish())
    {
        // ������ �����Լ�
        GetOwner()->GetScript<ER_DataScript_Character>()->CraftItem(param.iData[0]);

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Rio::CraftExit(tFSMData& param)
{
    param.bData[0] = false;
    param.iData[1] = 0;
    param.fData[0] = 0.f;
    STOPSOUND(CRAFT_SOUND);
}

void ER_ActionScript_Rio::AttackEnter(tFSMData& param)
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
    
    // Ȱ ��
    if (m_BowType)
    {
        Animator3D()->SelectAnimation(L"Rio_Long_Attack", false);
        param.iData[0] = 9;
    }
    else
    {
        Animator3D()->SelectAnimation(L"Rio_Short_Attack", false);
        param.iData[0] = 6;
    }

    // Ÿ�ٹ������� ȸ��
    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
    SetStateGrade(eAccessGrade::ADVANCED);
}
void ER_ActionScript_Rio::AttackUpdate(tFSMData& param)
{
    /*
    [ATTACK]
    bData[0]	: ���ݵ��� ���������� ����
    bData[1]	: Battle Event ���� ����
    bData[2]	: ���� Ÿ�� ���� ����
    bData[3]    : ���ݸ�� ����
    iData[0]	: Ÿ������ �ִϸ��̼� ������ = Hit Frame
    iData[1]    : ��ų�� ����
    lParam		: Ÿ�� ������Ʈ
    RParam		: Ÿ�� ���� ������Ʈ
    */

    float Atkspd = GetStatus()->fAttackSpeed;
    
    if (3 <= param.iData[1])
    {
        // ���� �ʱ�ȭ
        param.iData[1] = 0;
        
        // Q��ų 3���� ����ȿ��
        if (m_BowType)
        {
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_2);
            GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_ATK, 3.f, skill->Int1());
        }
        else
        {
            tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
            
            GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_APD, 3.f, skill->Float1());
            GetStatusEffect()->ActiveEffect((UINT)eStatus_Effect::INCREASE_SPD, 3.f, skill->Float2());
        }
    }

    // ����/����� Ȯ��
    tStatus_Effect* statusefc = GetStatusEffect();
    Atkspd += (Atkspd * statusefc->GetIncAPD()) - (Atkspd * statusefc->GetDecAPD());

    // �ִϸ��̼� �ӵ� ����
    Animator3D()->PlaySpeedValue(Atkspd);


    if (!param.bData[1] && param.iData[0] < Animator3D()->GetCurFrame())
    {
        // ���� ���
        if (m_BowType)
            ERCHARSOUND(LONG_NORMAL_ATTACK);
        else
            ERCHARSOUND(SHORT_NORMAL_ATTACK);

        // ĳ���� ���� ���� �˰���

        // ����ü ����
        CGameObject* Arrow = onew(CGameObject);
        ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
        Arrow->AddComponent(ArrowScript);
        
        ArrowScript->init();
        ArrowScript->SetForTarget(
            GetOwner(), 
            (CGameObject*)param.lParam, 
            GetProjSpawnPos(param.lParam), 
            15.f);
        ArrowScript->Spawn();

        param.bData[1] = true;                          // Battle Event �Ϸ�


        // ���� ���� 2Ÿ ����
        if (param.iData[0] < 11 && !m_BowType)
        {
            param.iData[0] = 11;
            param.bData[1] = false;
            SetStateGrade(eAccessGrade::ADVANCED);
        }
        
        SetStateGrade(eAccessGrade::BASIC);
    }


    if (Animator3D()->IsFinish())
    {
        param.iData[1]++;
        param.bData[0] = false;         // ���� ���� �Ϸ�

        // ������ Ÿ���� ����Ǿ���
        if (param.bData[2])
        {
            param.lParam    = param.RParam;
            param.bData[2]  = false;
            param.RParam    = 0;
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
void ER_ActionScript_Rio::AttackExit(tFSMData& param)
{
    param.bData[0] = false;
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Rio::Skill_Q(tFSMData& _Data)
{
    if (m_BowType)
    {
        STATEDATA_SET(SKILL_Q, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_Q);
    }
    else
    {
        STATEDATA_SET(SKILL_Q, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_Q);
    }
    

}
void ER_ActionScript_Rio::Skill_W(tFSMData& _Data)
{
    /*
    [ATTACK]
    bData[0]	: ���ݵ��� ���������� ����
    bData[1]	: Battle Event ���� ����
    bData[2]	: ���� Ÿ�� ���� ����
    iData[0]	: Ÿ������ �ִϸ��̼� ������ = Hit Frame
    lParam		: Ÿ�� ������Ʈ
    RParam		: Ÿ�� ���� ������Ʈ

    [MOVE]
    bData[0]	: Ÿ�� ���� ����
    fData		: ���� ���� �Ÿ�
    iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ��, 2 - �����۹ڽ�
    v4Data		: ��ǥ �̵� ��ǥ
    */

    // Ÿ�� ������Ʈ ������� �Ǵ�
    // ���� ���ݻ����� �����͸� �޾ƿ´�
    tFSMData PrevData = STATEDATA_GET(SKILL_W);

    //  ���� �������̾��°�
    if (PrevData.bData[0])
    {
        _Data.bData[0] = PrevData.bData[0];	// ���� ���� ����
        _Data.bData[1] = PrevData.bData[1];	// Hit ���� ����
        return;								// ���¸� �������� �ʰ� ����
    }
    else
    {
        STATEDATA_SET(SKILL_W, _Data);
        ChangeState(ER_CHAR_ACT::SKILL_W);
    }


}

void ER_ActionScript_Rio::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_E);
}
void ER_ActionScript_Rio::Skill_R(tFSMData& _Data)
{
    tFSMData PrevData = STATEDATA_GET(SKILL_R);

    if (PrevData.bData[0])
        return;

    _Data.bData[0] = PrevData.bData[0];
    _Data.bData[1] = PrevData.bData[1];
    _Data.iData[0] = PrevData.iData[0];
    _Data.v4Data = PrevData.v4Data;

    STATEDATA_SET(SKILL_R, _Data);
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Rio::Skill_QEnter(tFSMData& param)
{
    // ��ų ��Ÿ�� ����
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::Q_1);
    tSkill_Info* Skill2 = m_Data->GetSkill((UINT)SKILLIDX::Q_2);

    // ��� ��
    if (m_BowType)
    {
        if (Skill2->Use(&GetStatus()->iSP))
        {
            int tmpsp = 500;
            m_BowType = !m_BowType;
            Skill->Use(&tmpsp);
            Animator3D()->SelectAnimation(L"Rio_Long_Skill_Q", false);
            
            if (IsPlayer())
            {
                ER_UIMgr::GetInst()->SetToggle(0);
                ER_UIMgr::GetInst()->SetToggle(1);
                ER_UIMgr::GetInst()->SetToggle(3);
            }
        }
    }
    // �ܱ� ��
    else
    {
        if (Skill->Use(&GetStatus()->iSP))
        {
            int tmpsp = 500;
            m_BowType = !m_BowType;
            Skill2->Use(&tmpsp);
            Animator3D()->SelectAnimation(L"Rio_Short_Skill_Q", false);

            if (IsPlayer())
            {
                ER_UIMgr::GetInst()->SetToggle(0);
                ER_UIMgr::GetInst()->SetToggle(1);
                ER_UIMgr::GetInst()->SetToggle(3);
            }
        }
    }

}
void ER_ActionScript_Rio::Skill_QUpdate(tFSMData& param)
{
    if ((UINT)ER_CHAR_ACT::MOVE == m_iPrevState)
    {
        if (7 <= GetOwner()->Animator3D()->GetCurFrame())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::MOVE);
        }
    }
    else if (GetOwner()->Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Rio::Skill_QExit(tFSMData& param)
{
    SetStateGrade(eAccessGrade::BASIC);
}

void ER_ActionScript_Rio::Skill_WEnter(tFSMData& param)
{
    // ��ų ��Ÿ�� ����
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    tSkill_Info* Skill2 = m_Data->GetSkill((UINT)SKILLIDX::W_2);

    // ��� ��
    if (m_BowType)
    {
        if (Skill2->Use(&GetStatus()->iSP))
        {
            int tmpsp = 500;
            Skill->Use(&tmpsp);
            Animator3D()->SelectAnimation(L"Rio_Long_Skill_W", false);
            SetRotationToTarget(GetFocusPoint());
            SetStateGrade(eAccessGrade::ADVANCED);
            param.bData[0] = true;
            param.bData[1] = false;
        }
        else
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }
    // �ܱ� ��
    else
    {
        if (Skill->Use(&GetStatus()->iSP))
        {
            int tmpsp = 500;
            Skill2->Use(&tmpsp);
            Animator3D()->SelectAnimation(L"Rio_Short_Skill_W", false);
            SetRotationToTarget(GetFocusPoint());
            SetStateGrade(eAccessGrade::ADVANCED);
            param.bData[0] = true;
            param.bData[1] = false;
        }
        else
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }

}
void ER_ActionScript_Rio::Skill_WUpdate(tFSMData& param)
{
    // 11�����ӿ� ĵ���������� ���� (�ĵ����̻���)
    if (m_BowType)
    {
        // ���
        if (!param.bData[1] && 9 == Animator3D()->GetCurFrame())
        {
            Vec3 CenterDir = GetFocusDir();
            Vec3 vPos = Transform()->GetRelativePos();
            CenterDir.y -= 0.2618f * 2.f;
            Vec3 SpawnPos = {};

            CGameObject* Arrow = onew(CGameObject);
            ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
            Arrow->AddComponent(ArrowScript);
            ArrowScript->init();

            SpawnPos = vPos;
            SpawnPos.x += CenterDir.x * 0.5f;
            SpawnPos.z += CenterDir.z * 0.5f;
            SpawnPos.y = SpawnPos.y + 1.1f;

            ArrowScript->SetForDir(GetOwner(), SpawnPos, 15.f, 0.7f);
            ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillW2);
            ArrowScript->Spawn();

            param.bData[1] = true;                          // Battle Event �Ϸ�

            SetStateGrade(eAccessGrade::BASIC);
        }
    }
    else
    {
        // �ܱ�
        if (!param.bData[1] && 11 == Animator3D()->GetCurFrame())
        {
            // ����ü ����
            // 7.5�� : 0.1309f

            float fRad = 0.2618f;
            Vec3 CenterDir = GetFocusDir();
            Vec3 vPos = Transform()->GetRelativePos();
            CenterDir.y -= 0.2618f * 2.f;
            Vec3 SpawnPos = {};
            
            for (int i = 0; i < 5; ++i)
            {
                CGameObject* Arrow = onew(CGameObject);
                ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
                Arrow->AddComponent(ArrowScript);
                ArrowScript->init();
                
                float angle = CenterDir.y + (fRad * i);                                 // i��° ȭ�쿡 ���� ���� ���
                float newX = CenterDir.x * cos(angle) - CenterDir.z * sin(angle);       // x���� ����
                float newZ = CenterDir.x * sin(angle) + CenterDir.z * cos(angle);       // z���� ����
                Vec3 Direction = Vec3(newX, CenterDir.y, newZ);                         // ����� ���� ���� ����

                SpawnPos = vPos;
                SpawnPos.x += Direction.x * 0.5f;
                SpawnPos.z += Direction.z * 0.5f;
                SpawnPos.y = SpawnPos.y + 1.1f;

                ArrowScript->SetForDir(GetOwner(), SpawnPos, 15.f, 0.4f);
                ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillW1);
                ArrowScript->Spawn();
            }

            param.bData[1] = true;                          // Battle Event �Ϸ�

            //5�� ������ �߻�
            SetStateGrade(eAccessGrade::BASIC);
        }
    }

    // ��ų �ִϸ��̼��� ������ �����·� �ڵ� ��ȯ.
    if (Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Rio::Skill_WExit(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[1] = false;
}

void ER_ActionScript_Rio::Skill_EEnter(tFSMData& param)
{
    // ��ų ��Ÿ�� ����
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);

    if (Skill->Use(&GetStatus()->iSP))
    {
        Animator3D()->SelectAnimation(L"Rio_Short_Skill_E", false);

        SetRotationToTarget(GetFocusPoint());
        SetStateGrade(eAccessGrade::UTMOST);

        // ���� ��ȯ
        SetRotationToTarget(param.v4Data);

        // ���� ����
        Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
        Vec3 vDir = (param.v4Data - vPos).Normalize();

        // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
        param.v2Data = Vec2(vDir.x, vDir.z);

        param.fData[0] = 10.f;                                              // ��ų �Ÿ�
        float ClearDist = GetClearDistance(vDir, param.fData[0]);
        param.fData[1] = ClearDist;                                         // �̵� ���� �Ÿ�
        param.fData[2] = (float)Animator3D()->GetCurAnim()->GetAnimClip().dEndTime;   // ��ü �ִϸ��̼� ��� �ð�
        param.fData[3] = 0.f;                                              // �̵��� �Ÿ� �ʱ�ȭ.
        param.bData[0] = false;
    }
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Rio::Skill_EUpdate(tFSMData& param)
{
    // param.v4Data[0] : ��ų �Ÿ�(�ӵ�)
    // param.v4Data[1] : �̵� ���� �Ÿ�
    // param.v4Data[2] : ��ü �ִϸ��̼� ��� �ð�
    // param.v4Data[3] : �̵��� �Ÿ�

    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vDir(param.v2Data.x, 0.f, param.v2Data.y);
    int CurFmame = Animator3D()->GetCurFrame();

    if (CurFmame < 3)
    {
        if (param.fData[3] < param.fData[0] * 0.1f)
        {
            float speed = param.fData[0] * 0.5f;

            float CurFrameMoveDist = speed * param.fData[2] * DT;

            param.fData[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            Transform()->SetRelativePos(vPos);
        }
        return;
    }
    // 3 ����
    else if (CurFmame < 16)
    {
        // ���ݽ����� �����Ǻ��� ĳ���͵� �Ÿ��˻�
        if (!param.bData[0] && 11 < CurFmame)
        {
            vector<CGameObject*> vecChar = ER_GameSystem::GetInst()->GetCharacters();
            
            for (auto Target : vecChar)
            {
                // ���� �˻�
                if (Target == GetOwner())
                    continue;

                // �Ÿ� �˻�
                Vec3 TargetPos = Target->Transform()->GetRelativePos();
                float dist = Vec3::Distance(vPos, TargetPos);

                if (5.f < dist)
                    continue;

                // ��ų���� ����

                // ��� ��
                if (m_BowType)
                {
                    CGameObject* Arrow = onew(CGameObject);
                    ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
                    Arrow->AddComponent(ArrowScript);
                    ArrowScript->init();

                    ArrowScript->SetForTarget(GetOwner(), Target, vPos, 15.f);
                    ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillE2);
                    ArrowScript->Spawn();

                }
                // �ܱ� ��
                else
                {
                    CGameObject* Arrow = onew(CGameObject);
                    ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
                    Arrow->AddComponent(ArrowScript);
                    ArrowScript->init();

                    ArrowScript->SetForTarget(GetOwner(), Target, vPos, 15.f);
                    ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillE1);
                    ArrowScript->Spawn();
                }
            }

            param.bData[0] = true;                          // Battle Event �Ϸ�
        }

        // �̵��ѰŸ��� �̵����ɰŸ��� ���� �ʾҴ��� �Ǵ�.
        if (param.fData[3] < param.fData[1])
        {
            float speed = param.fData[0] * 1.5f;

            float CurFrameMoveDist = speed * param.fData[2] * DT;

            param.fData[3] += CurFrameMoveDist;

            vPos += vDir * CurFrameMoveDist;

            // ĳ���� �̵�
            Transform()->SetRelativePos(vPos);
        }
        return;
    }
    // 17 ����

    if (Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
        // clear
        param.fData[0] = 0.f;
        param.fData[1] = 0.f;
        param.fData[2] = 0.f;
        param.fData[3] = 0.f;
        param.v2Data = Vec2();
     
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}
void ER_ActionScript_Rio::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Rio::Skill_REnter(tFSMData& param)
{
    // ��ų ��Ÿ�� ����
    tSkill_Info* Skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    tSkill_Info* Skill2 = m_Data->GetSkill((UINT)SKILLIDX::R_2);

    // ��� ��
    if (m_BowType)
    {
        if (Skill2->Use(&GetStatus()->iSP))
        {
            int tmpsp = 500;
            Skill->Use(&tmpsp);
            Animator3D()->SelectAnimation(L"Rio_Long_Skill_R", false);
            SetRotationToTarget(GetFocusPoint());
            
            param.iData[0] = 0;
            param.bData[0] = true;
            param.bData[1] = false;

            param.iData[1] = 39;
            SetStateGrade(eAccessGrade::UTMOST);
        }
        else
        {
            ChangeState(ER_CHAR_ACT::WAIT);
            return;
        }
    }
    // �ܱ� ��
    else
    {
        // ù ����
        if (0 == param.iData[0])
        {
            if (Skill->Use(&GetStatus()->iSP))
            {
                int tmpsp = 500;
                Skill2->Use(&tmpsp);
                Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_Start", false);
                SetRotationToTarget(GetFocusPoint());


                Vec3 CenterDir = GetFocusDir();
                Vec3 vPos = Transform()->GetRelativePos();
                Vec3 SpawnPos = {};
                SpawnPos = vPos;
                SpawnPos.x += CenterDir.x * 0.5f;
                SpawnPos.z += CenterDir.z * 0.5f;
                SpawnPos.y = SpawnPos.y + 1.1f;

                param.v4Data = SpawnPos;

                param.bData[0] = true;
                param.bData[1] = false;
                param.iData[1] = 9; // ����ü ������
                SetStateGrade(eAccessGrade::UTMOST);
            }
            else
            {
                ChangeState(ER_CHAR_ACT::WAIT);
                return;
            }

        }
        else if (1 == param.iData[0])
        {
            param.bData[0] = true;
            param.bData[1] = false;
            param.iData[1] = 9;
            // �ι�° ����
            Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_End", false);
            SetRotationToTarget(GetFocusPoint());
            SetStateGrade(eAccessGrade::UTMOST);
        }
        else
        {
            ChangeState(ER_CHAR_ACT::WAIT);
            return;
        }
    }


}
void ER_ActionScript_Rio::Skill_RUpdate(tFSMData& param)
{
    // ��� ��
    if (m_BowType)
    {
        if (!param.bData[1] && param.iData[1] < Animator3D()->GetCurFrame())
        {
            CGameObject* Arrow = onew(CGameObject);
            ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
            Arrow->AddComponent(ArrowScript);
            ArrowScript->init();
            
            Arrow->Collider3D()->SetOffsetScale(Vec3(0.4f, 0.2f, 0.4f));
            Arrow->Transform()->SetRelativeScale(Vec3(4.f, 2.f, 4.f));


            ArrowScript->SetForDir(GetOwner(), param.v4Data, 12.f, 5.f);
            ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillR2);
            ArrowScript->Spawn();

            param.bData[1] = true;
        }

        if (Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }
    // �ܱ� ��
    else
    {
        switch (param.iData[0])
        {
        case 0: // ���� ����
        {
            if (!param.bData[1] && param.iData[1] < Animator3D()->GetCurFrame())
            {
                CGameObject* Arrow = onew(CGameObject);
                ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
                Arrow->AddComponent(ArrowScript);
                ArrowScript->init();

                ArrowScript->SetForDir(GetOwner(), param.v4Data, 15.f, 0.4f);
                ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillR1_1);
                ArrowScript->Spawn();

                param.bData[1] = true;

                if (9 == param.iData[1])
                {
                    param.iData[1] = 16;
                    param.bData[1] = false;
                }
                else if (16 == param.iData[1])
                {
                    param.iData[1] = 23;
                    param.bData[1] = false;
                }
            }

            if (Animator3D()->IsFinish())
            {
                SetStateGrade(eAccessGrade::BASIC);
                ChangeState(ER_CHAR_ACT::WAIT);
                param.iData[0]++;
            }
            break;
        }
        case 1: // ���ᵿ��
        {
            // �ִϸ��̼� ���̸�ŭ ���������� UI ���
            if (!param.bData[1] && param.iData[1] < Animator3D()->GetCurFrame())
            {
                Vec3 CenterDir = GetFocusDir();
                Vec3 vPos = Transform()->GetRelativePos();
                Vec3 SpawnPos = {};

                CGameObject* Arrow = onew(CGameObject);
                ER_DataScript_Arrow* ArrowScript = onew(ER_DataScript_Arrow);
                Arrow->AddComponent(ArrowScript);
                ArrowScript->init();

                SpawnPos = vPos;
                SpawnPos.x += CenterDir.x * 0.5f;
                SpawnPos.z += CenterDir.z * 0.5f;
                SpawnPos.y = SpawnPos.y + 1.1f;

                ArrowScript->SetForDir(GetOwner(), SpawnPos, 15.f, 0.4f);
                ArrowScript->SetSkill(this, (SKILL_DMG_CALC)&ER_ActionScript_Rio::SkillR1_2);
                ArrowScript->Spawn();

                param.bData[1] = true;
            }
            // 1��

            if (Animator3D()->IsFinish())
            {
                SetStateGrade(eAccessGrade::BASIC);
                ChangeState(ER_CHAR_ACT::WAIT);
                param.iData[0] = 0;
                param.bData[0] = false;
                param.bData[1] = false;
                param.v4Data = Vec4(0.f,0.f,0.f,0.f);
            }
            break;
        }
        }
    }
}
void ER_ActionScript_Rio::Skill_RExit(tFSMData& param)
{
    param.bData[0] = false;
}


int ER_ActionScript_Rio::SkillW1()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::W_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.9f);

    return Dmg;
}

int ER_ActionScript_Rio::SkillW2()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::W_2);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.8f);
    return Dmg;
}

int ER_ActionScript_Rio::SkillE1()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.35f);
    
    return Dmg;
}

int ER_ActionScript_Rio::SkillE2()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::E_2);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.4f);

    return Dmg;
}

int ER_ActionScript_Rio::SkillR1_1()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.4f);

    return Dmg;
}

int ER_ActionScript_Rio::SkillR1_2()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_1);
    int Dmg = (int)(skill->Int2() + GetStatus()->iAttackPower * 0.8f);

    return Dmg;
}

int ER_ActionScript_Rio::SkillR2()
{
    tSkill_Info* skill = m_Data->GetSkill((UINT)SKILLIDX::R_2);
    int Dmg = (int)(skill->Int1() + GetStatus()->iAttackPower * 0.4f);

    return Dmg;
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
FSMState* ER_ActionScript_Rio::CreateFarming()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Rio, Farming);
    STATEDELEGATE_UPDATE(state, Rio, Farming);
    STATEDELEGATE_EXIT(state, Rio, Farming);

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