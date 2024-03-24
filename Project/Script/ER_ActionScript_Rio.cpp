#include "pch.h"
#include "ER_ActionScript_Rio.h"
#include "ER_DataScript_Character.h"
#include "ER_ProjectilePool.h"
#include "ER_ProjectileScript.h"

#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine\CAnim3D.h>

ER_ActionScript_Rio::ER_ActionScript_Rio()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO)
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
    param.fData = 0.f;
}
void ER_ActionScript_Rio::WaitUpdate(tFSMData& param)
{
    // 0.5�ʸ��� ȸ��
    param.fData += DT;
    
    if (0.5f <= param.fData)
    {
        // HP/SP �ڿ� ȸ��
        m_Data->HPRegen();
        m_Data->SPRegen();

        // �ڿ���� ī��Ʈ �ʱ�ȭ
        param.fData -= 0.5f;
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
    param.fData = 0.f;
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
    SetStateGrade(eAccessGrade::UTMOST);
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
        if (m_BowType)
            ERCHARSOUND(LONG_NORMAL_ATTACK);
        else
            ERCHARSOUND(SHORT_NORMAL_ATTACK);

        // ĳ���� ���� ���� �˰���

        // ����ü ����
        ER_ProjectileScript* Arrow = GETPROJECTILE(ARROW);
        
        Arrow->ShotTarget(GetOwner(),                   // ���� ������Ʈ
            (CGameObject*)param.lParam,                 // Ÿ�� ������Ʈ
            GetProjSpawnPos(param.lParam),              // ����ü ���� ��ġ
            ER_ProjectileScript::eDmgType::NORMAL,      // ������ Ÿ��
            15.f);                                      // ����ü �ӵ�
        
        Arrow->Shot();                                  // ����ü �߻�

        param.bData[1] = true;                          // Battle Event �Ϸ�


        // ���� ���� 2Ÿ ����
        if (param.iData[0] < 11 && !m_BowType)
        {
            param.iData[0] = 11;
            param.bData[1] = false;
        }
        SetStateGrade(eAccessGrade::BASIC);
    }


    if (Animator3D()->IsFinish())
    {
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

void ER_ActionScript_Rio::FarmingEnter(tFSMData& param)
{
    if (m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Run", true);

    SetStateGrade(eAccessGrade::BASIC);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}
void ER_ActionScript_Rio::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        if (m_BowType)
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Wait", true);
        else
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Wait", true);

    Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 ObjectPos = ((CGameObject*)param.lParam)->Transform()->GetRelativePos();

    XMVECTOR vRangeScale = XMVector3Length(ownerPos - ObjectPos);
    float rangeScale = XMVectorGetX(vRangeScale);

    if (abs(rangeScale) < 2.0f && m_bFarmingTrigger) {
        // Vec3 posResult = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
        // ER_UIMgr::GetInst()->GetItemBoxBackground()->SetEnable(true);
        // ER_UIMgr::GetInst()->GetItemBoxBackground()->Transform()->SetRelativePos(Vec3(posResult.x, posResult.y - 100.f, -1.0f));

        // vector<CGameObject*> itemList = ((CGameObject*)param.lParam)->GetScript<ER_DataScript_ItemBox>()->GetItemList();
        // for (int i = 0; i < itemList.size(); ++i) {
        //     if (itemList[i]) {
        //         std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList((int)i / 4, (int)i % 4);
        // 
        //         itemLists.first->SetEnable(true);
        //         itemLists.second->SetEnable(true);
        // 
        //         itemLists.first->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_UIMgr::GetInst()->GetGradeTexture(itemList[i]->GetScript<ER_DataScript_Item>()->GetGrade()));
        //         itemLists.second->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, itemList[i]->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
        //     }
        // }

        m_bFarmingTrigger = false;
    }
}
void ER_ActionScript_Rio::FarmingExit(tFSMData& param)
{
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            // std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList(i, j);

            // itemLists.first->SetEnable(false);
            // itemLists.second->SetEnable(false);
        }
    }

    m_pFarmingObject = nullptr;
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

void ER_ActionScript_Rio::Skill_QEnter(tFSMData& param)
{
    // Bow Ÿ�� ��ȯ
    m_BowType = !m_BowType;

    if(m_BowType)
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Long_Skill_Q", false);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_Q", false);

    SetStateGrade(eAccessGrade::ADVANCED);
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
}

void ER_ActionScript_Rio::Skill_WEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    if (m_BowType)
        Animator->SelectAnimation(L"Rio_Long_Skill_W", false);
    else
        Animator->SelectAnimation(L"Rio_Short_Skill_W", false);

    SetStateGrade(eAccessGrade::ADVANCED);
}
void ER_ActionScript_Rio::Skill_WUpdate(tFSMData& param)
{
    // 11�����ӿ� ĵ���������� ���� (�ĵ����̻���)
    if (11 == GetOwner()->Animator3D()->GetCurFrame())
        SetStateGrade(eAccessGrade::BASIC);

    // ��ų �ִϸ��̼��� ������ �����·� �ڵ� ��ȯ.
    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetStateGrade(eAccessGrade::BASIC);
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

    SetStateGrade(eAccessGrade::ADVANCED);
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
        SetStateGrade(eAccessGrade::BASIC);
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
        param.iData[0] = 0;
    }
    else
    {
        // ù ����
        if(0 == param.iData[0])
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_Start", false);

        // �ι�° ����
        if(1 == param.iData[0])
            GetOwner()->Animator3D()->SelectAnimation(L"Rio_Short_Skill_R_End", false);
    }

    SetStateGrade(eAccessGrade::UTMOST);
}
void ER_ActionScript_Rio::Skill_RUpdate(tFSMData& param)
{
    // ��� ��
    if (m_BowType)
    {
        if (GetOwner()->Animator3D()->IsFinish())
        {
            SetStateGrade(eAccessGrade::BASIC);
            ChangeState(ER_CHAR_ACT::WAIT);
        }
    }
    // �ܱ� ��
    else
    {
        CAnimator3D* animator = GetOwner()->Animator3D();

        switch (param.iData[0])
        {
        case 0: // ���� ����
        {
            // �ִϸ��̼� ���̸�ŭ ���������� UI ���

            // 3����

            if (animator->IsFinish())
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

            // 1��

            if (animator->IsFinish())
            {
                SetStateGrade(eAccessGrade::BASIC);
                ChangeState(ER_CHAR_ACT::WAIT);
                param.iData[0] = 0;
            }
            break;
        }
        }
    }
}
void ER_ActionScript_Rio::Skill_RExit(tFSMData& param)
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