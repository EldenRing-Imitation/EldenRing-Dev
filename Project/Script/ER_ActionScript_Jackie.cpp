#include "pch.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_DataScript_Character.h"

#include "ER_UIMgr.h"
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

FSMState* ER_ActionScript_Jackie::CreateWait()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Wait);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateMove()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Move);
    STATEDELEGATE_UPDATE(state, Jackie, Move);

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

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateRest()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Rest);
    STATEDELEGATE_UPDATE(state, Jackie, Rest);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateAttack()
{
    FSMState* state = new FSMState(this);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateArrive()
{
    FSMState* state = new FSMState(this);

    STATEDELEGATE_ENTER(state, Jackie, Arrive);
    STATEDELEGATE_UPDATE(state, Jackie, Arrive);

    return state;
}

FSMState* ER_ActionScript_Jackie::CreateDeath()
{
    FSMState* state = new FSMState(this);

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

void ER_ActionScript_Jackie::tick()
{
    // W��ų ���ӽð� üũ
    {
        tFSMData skillW = STATEDATA_GET(SKILL_W);
        tFSMData skillR = STATEDATA_GET(SKILL_R);

        if (skillW.iData)
        {
            skillW.fData -= DT;
            
            // W��ų ���ӽð��� ���� ���
            if (skillW.fData <= 0)
            {
                skillW.iData = 0;
                skillW.fData = 0.f;
                
                // ��ų ���ӽð��� ������ �� MOVE ���¶�� �̵���� ����
                if(m_iCurState == ER_CHAR_ACT::MOVE)
                    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
            }

            STATEDATA_SET(SKILL_W, skillW);
        }

        if (skillR.iData)
        {
            skillR.fData -= DT;

            // W��ų ���ӽð��� ���� ���
            if (skillR.fData <= 0)
            {
                skillR.iData = 0;
                skillR.fData = 0.f;

                // ��ų ���ӽð��� ������ �� MOVE ���¶�� �̵���� ����
                if (m_iCurState == ER_CHAR_ACT::MOVE)
                    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
            }

            STATEDATA_SET(SKILL_R, skillR);
        }


    }

    ER_ActionScript_Character::tick();
}

void ER_ActionScript_Jackie::Attack(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Wait(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Jackie::Farming(tFSMData& _Data)
{
    STATEDATA_SET(FARMING, _Data);

    ChangeState(ER_CHAR_ACT::FARMING);
}

void ER_ActionScript_Jackie::Craft(tFSMData& _Data)
{
}

void ER_ActionScript_Jackie::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Jackie::Skill_Q(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Jackie::Skill_W(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Jackie::Skill_E(tFSMData& _Data)
{
    // �������� �ƴ϶��
    if (!STATEDATA_GET(SKILL_E).iData)
        STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E);
}

void ER_ActionScript_Jackie::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Jackie::MoveEnter(tFSMData& param)
{
    if(0 == STATEDATA_GET(SKILL_W).iData)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Jackie::MoveUpdate(tFSMData& param)
{
    // ĳ���� �ӵ� ���ͼ� �־��ֱ�
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // W��ų �����ִ°�� �̵��ӵ� ����
    if (1 == STATEDATA_GET(SKILL_W).iData)
        speed *= 1.3f;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::FarmingEnter(tFSMData& param)
{
    if (0 == STATEDATA_GET(SKILL_W).iData)
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Run", true);
    else
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_SkillW", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}

void ER_ActionScript_Jackie::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 ObjectPos = ((CGameObject*)param.lParam)->Transform()->GetRelativePos();

    XMVECTOR vRangeScale = XMVector3Length(ownerPos - ObjectPos);
    float rangeScale = XMVectorGetX(vRangeScale);

    if (abs(rangeScale) < 2.0f && m_bFarmingTrigger) {
        Vec3 posResult = ER_UIMgr::GetInst()->WorldPosToUIPos(GetOwner()->Transform()->GetRelativePos());
        ER_UIMgr::GetInst()->GetItemBoxBackground()->SetEnable(true);
        ER_UIMgr::GetInst()->GetItemBoxBackground()->Transform()->SetRelativePos(Vec3(posResult.x, posResult.y - 100.f, -1.0f));

        vector<CGameObject*> itemList = ((CGameObject*)param.lParam)->GetScript<ER_DataScript_ItemBox>()->GetItemList();
        for (int i = 0; i < itemList.size(); ++i) {
            if (itemList[i]) {
                std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList((int)i / 4, (int)i % 4);

                itemLists.first->SetEnable(true);
                itemLists.second->SetEnable(true);

                itemLists.first->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, ER_UIMgr::GetInst()->GetGradeTexture(itemList[i]->GetScript<ER_DataScript_Item>()->GetGrade()));
                itemLists.second->MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, itemList[i]->GetScript<ER_DataScript_Item>()->GetItemTex().Get());
            }
        }

        m_bFarmingTrigger = false;
    }
}

void ER_ActionScript_Jackie::FarmingExit(tFSMData& param)
{
    ER_UIMgr::GetInst()->GetItemBoxBackground()->SetEnable(false);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::pair<CGameObject*, CGameObject*> itemLists = ER_UIMgr::GetInst()->GetItemBoxList(i, j);

            itemLists.first->SetEnable(false);
            itemLists.second->SetEnable(false);
        }
    }

    m_pFarmingObject = nullptr;
}

void ER_ActionScript_Jackie::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Jackie::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Arrive", false);
}

void ER_ActionScript_Jackie::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Jackie::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Jackie_Rest_Loop");

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
            animator->SelectAnimation(L"Jackie_Rest_End", false);
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

void ER_ActionScript_Jackie::Skill_QEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ����
}

void ER_ActionScript_Jackie::Skill_QUpdate(tFSMData& param)
{
    switch (param.iData)
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
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
        }
        break;
    }
    case 2:
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

void ER_ActionScript_Jackie::Skill_QExit(tFSMData& param)
{
    param.iData = 0;
}

void ER_ActionScript_Jackie::Skill_WEnter(tFSMData& param)
{
    param.iData = 1;    // ��ų ON
    param.fData = 3.f;  // ���ӽð�
}

void ER_ActionScript_Jackie::Skill_WUpdate(tFSMData& param)
{
    SetAbleToCancle(bAbleChange::COMMON);

    if (ER_CHAR_ACT::MOVE == m_iPrevState)
        ChangeState(ER_CHAR_ACT::MOVE);
    else
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Jackie::Skill_EEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Jackie_Wait", true);
    param.iData = 1; // 0. �⺻, 1. ��ų ����, 2. ��ų ����
}

void ER_ActionScript_Jackie::Skill_EUpdate(tFSMData& param)
{
    CTransform* transform = GetOwner()->Transform();
    CAnimator3D* animator = GetOwner()->Animator3D();

    switch (param.iData)
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
            SetAbleToCancle(bAbleChange::DISABLE);
            param.iData++;
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
            param.iData++;
        }

        break;
    }
    case 3:
    {
        // ���
        // 

        if (GetOwner()->Animator3D()->IsFinish())
            ChangeState(ER_CHAR_ACT::WAIT, bAbleChange::DISABLE);

        break;
    }

    default:
        break;
    }
}

void ER_ActionScript_Jackie::Skill_EExit(tFSMData& param)
{
    param.iData = 0;
}

void ER_ActionScript_Jackie::Skill_REnter(tFSMData& param)
{
    param.iData = 1;    // ��ų ON
    param.fData = 5.f;  // ���ӽð�
}

void ER_ActionScript_Jackie::Skill_RUpdate(tFSMData& param)
{
}

void ER_ActionScript_Jackie::Skill_RExit(tFSMData& param)
{
}
