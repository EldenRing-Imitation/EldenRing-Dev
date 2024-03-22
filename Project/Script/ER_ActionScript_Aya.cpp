#include "pch.h"
#include "ER_ActionScript_Aya.h"
#include "ER_DataScript_Character.h"

#include "ER_UIMgr.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_Item.h"

#include <Engine/CAnim3D.h>

ER_ActionScript_Aya::ER_ActionScript_Aya()
    : ER_ActionScript_Character(SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA)
    , m_fSec(0.f)
{
}

ER_ActionScript_Aya::~ER_ActionScript_Aya()
{
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

void ER_ActionScript_Aya::Attack(tFSMData& _Data)
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

void ER_ActionScript_Aya::Wait(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::Move(tFSMData& _Data)
{
    STATEDATA_SET(MOVE, _Data);

    ER_ActionScript_Character::Move(_Data);
}

void ER_ActionScript_Aya::Farming(tFSMData& _Data)
{
    STATEDATA_SET(FARMING, _Data);

    ChangeState(ER_CHAR_ACT::FARMING);
}

void ER_ActionScript_Aya::Craft(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::CRAFT);
}

void ER_ActionScript_Aya::Rest(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::REST);
}

void ER_ActionScript_Aya::Skill_Q(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_Q, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_Q);
}

void ER_ActionScript_Aya::Skill_W(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_W, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_W);
}

void ER_ActionScript_Aya::Skill_E(tFSMData& _Data)
{
    STATEDATA_SET(SKILL_E, _Data);

    ChangeState(ER_CHAR_ACT::SKILL_E, bAbleChange::ABSOUTE);
}

void ER_ActionScript_Aya::Skill_R(tFSMData& _Data)
{
    ChangeState(ER_CHAR_ACT::SKILL_R);
}

void ER_ActionScript_Aya::MoveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Run");
    
    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    bool bMove = findpathcomp->FindPath(DestPos);
}

void ER_ActionScript_Aya::MoveUpdate(tFSMData& param)
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

void ER_ActionScript_Aya::MoveExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::FarmingEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Run", true);

    SetAbleToCancle(bAbleChange::COMMON);

    Vec3 DestPos = param.v4Data;

    CFindPath* findpathcomp = GetOwner()->FindPath();
    findpathcomp->FindPath(((CGameObject*)param.lParam)->Transform()->GetRelativePos());

    m_pFarmingObject = (CGameObject*)param.lParam;

    m_bFarmingTrigger = true;
}

void ER_ActionScript_Aya::FarmingUpdate(tFSMData& param)
{
    float speed = m_Data->GetStatus()->fMovementSpeed;

    // ���� �̵������� ���ٸ� �����·� ��ȯ
    if (!GetOwner()->FindPath()->PathMove(speed))
        GetOwner()->Animator3D()->SelectAnimation(L"Aya_Idle", true);

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

void ER_ActionScript_Aya::FarmingExit(tFSMData& param)
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

void ER_ActionScript_Aya::WaitEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Idle", true);

    SetAbleToCancle(bAbleChange::COMMON);
}

void ER_ActionScript_Aya::WaitUpdate(tFSMData& param)
{
}

void ER_ActionScript_Aya::WaitExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::ArriveEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Arrive", false);
}

void ER_ActionScript_Aya::ArriveUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
        ChangeState(ER_CHAR_ACT::WAIT);
}

void ER_ActionScript_Aya::ArriveExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::AttackEnter(tFSMData& param)
{
    param.bData[0] = false;
    param.bData[2] = false;

    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Attack", false);

    SetRotationToTarget(((CGameObject*)param.lParam)->Transform()->GetRelativePos());
}

void ER_ActionScript_Aya::AttackUpdate(tFSMData& param)
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
    int HitFrame = param.bData[3] ? 8 : 8;
    if (!param.bData[2] && animator->GetCurFrame() < HitFrame)
    {
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

void ER_ActionScript_Aya::AttackExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::CraftEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Craft", false);
}

void ER_ActionScript_Aya::CraftUpdate(tFSMData& param)
{
    if (GetOwner()->Animator3D()->IsFinish())
    {
        // ������ �����Լ�
        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::CraftExit(tFSMData& param)
{

}

void ER_ActionScript_Aya::RestEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Rest_Start", false);

    param.iData = 0;
}

void ER_ActionScript_Aya::RestUpdate(tFSMData& param)
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
            animator->SelectAnimation(L"Aya_Rest_Loop");

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
            animator->SelectAnimation(L"Aya_Rest_End", false);
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

void ER_ActionScript_Aya::RestExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_QEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_SkillQ", false);
    SetRotationToTarget(param.v4Data);
    SetAbleToCancle(bAbleChange::DISABLE);
}

void ER_ActionScript_Aya::Skill_QUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();

    int curFrame = animator->GetCurFrame();

    if (animator->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
    }

    if (curFrame > 16) {
        SetAbleToCancle(bAbleChange::COMMON);
        param.iData = 0;
    }
}

void ER_ActionScript_Aya::Skill_QExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_WEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Aya_SkillW_Shot", false);

    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;
    param.v2Data.Normalize();

    param.v4Data.x = vPos.x;
    param.v4Data.y = vPos.y;
    param.v4Data.z = vPos.z;

    param.iData = 0;
    param.fData = 0.f;

    param.bData[0] = false;

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Aya::Skill_WUpdate(tFSMData& param)
{
    m_fSec += DT;

    CAnimator3D* Animator = GetOwner()->Animator3D();

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
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
        float angleDeg = angleRad * (180.0 / XM_PI);

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
        param.fData = angleDeg;

        param.bData[0] = true;

        GetOwner()->FindPath()->FindPath(param.v4Data);
    }

    //�ִϸ��̼��� ���������� �̵� �ִϸ��̼� ����
    if ((abs(vPos.x- param.v4Data.x)>0.1f || abs(vPos.z - param.v4Data.z)>0.1f) && param.bData[0]) {
        // ���� ����
        if (param.fData > -45 && param.fData <= 45) {
            if (param.iData != 1) {
                Animator->SelectAnimation(L"Aya_SkillW_Forward", true);
                param.iData = 1;
            }
        }
        else if (param.fData > 45 && param.fData <= 135) {
            if (param.iData != 2) {
                Animator->SelectAnimation(L"Aya_SkillW_Left", true);
                param.iData = 2;
            }
        }
        else if (param.fData > -135 && param.fData <= -45) {
            if (param.iData != 3) {
                Animator->SelectAnimation(L"Aya_SkillW_Right", true);
                param.iData = 3;
            }
        }
        else {
            if (param.iData != 4) {
                Animator->SelectAnimation(L"Aya_SkillW_Back", true);
                param.iData = 4;
            }
        }
        param.bData[0] = false;
    }
    //������ �׳� ��.
    else if((abs(vPos.x - param.v4Data.x) < 0.1f && abs(vPos.z - param.v4Data.z) < 0.1f)){
        Animator->SelectAnimation(L"Aya_SkillW_Shot", false);
        param.iData = 0;
    }

    //������ ��ġ�� ���� ��ġ�� ���� ���� ��, ��� �̵���.
    if (vPos != param.v4Data) {
        float speed = m_Data->GetStatus()->fMovementSpeed;
        GetOwner()->FindPath()->PathMove(speed, false);
    }

    if (m_fSec > 1.5f) {
        SetAbleToCancle(bAbleChange::COMMON);
        ChangeState(ER_CHAR_ACT::WAIT);
        param.iData = 0;
        param.fData = 0.f;
        param.v2Data = Vec2();
        param.v4Data = Vec4();
        m_fSec = 0.f;
    }
}

void ER_ActionScript_Aya::Skill_WExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_EEnter(tFSMData& param)
{
    CAnimator3D* Animator = GetOwner()->Animator3D();
    Animator->SelectAnimation(L"Aya_SkillE", false);

    SetRotationToTarget(param.v4Data);

    // ���� ����
    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    Vec3 vDir = (param.v4Data - vPos).Normalize();

    // param.v4Data �� �޾ƿ� ���������� v2Data�� �̵�
    param.v2Data.x = vDir.x;
    param.v2Data.y = vDir.z;

    param.v4Data[0] = 8.f;                                              // ��ų �Ÿ�
    float ClearDist = GetClearDistance(vDir, param.v4Data[0]);
    param.v4Data[1] = ClearDist;                                        // �̵� ���� �Ÿ�
    param.v4Data[2] = (float)Animator->GetCurAnim()->GetAnimClip().dEndTime;   // ��ü �ִϸ��̼� ��� �ð�
    param.v4Data[3] = 0.f;                                              // �̵��� �Ÿ� �ʱ�ȭ.

    SetAbleToCancle(bAbleChange::ABSOUTE);
}

void ER_ActionScript_Aya::Skill_EUpdate(tFSMData& param)
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
            speed = param.v4Data[0] * 1.5f;

        float CurFrameMoveDist = speed * param.v4Data[2] * DT;

        param.v4Data[3] += CurFrameMoveDist;

        vPos += vDir * CurFrameMoveDist;

        // ĳ���� �̵�
        transform->SetRelativePos(vPos);
    }
    else {
        //������ �ִϸ��̼� ��¾��ص� ��ǥ��ġ�� �����ϸ� ĵ�� ����
        SetAbleToCancle(bAbleChange::COMMON);
        param.v4Data = Vec4();
        param.v4Data = Vec2();
        param.iData = 0;
    }

    if (GetOwner()->Animator3D()->IsFinish())
    {
        SetAbleToCancle(bAbleChange::COMMON);
        // clear
        param.v4Data = Vec4();
        param.v4Data = Vec2();

        ChangeState(ER_CHAR_ACT::WAIT);
    }
}

void ER_ActionScript_Aya::Skill_EExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_REnter(tFSMData& param)
{
}

void ER_ActionScript_Aya::Skill_RUpdate(tFSMData& param)
{
    CAnimator3D* animator = GetOwner()->Animator3D();
    switch (param.iData)
    {
    //���� �� ĳ���Ұ�
    case 0:
    {
        animator->SelectAnimation(L"Aya_SkillR_Start", false);
        SetAbleToCancle(bAbleChange::DISABLE);
        ++param.iData;
    }
        break;
    //�ĵ� ĳ�� ����
    case 1:
    {
        if (animator->IsFinish())
        {
            animator->SelectAnimation(L"Aya_SkillR_End", false);
            SetAbleToCancle(bAbleChange::COMMON);
            ++param.iData;
            param.iData = 0;
        }
    }
        break;
    case 2:
    {
        if (animator->IsFinish())
        {
            SetAbleToCancle(bAbleChange::COMMON);
            ChangeState(ER_CHAR_ACT::WAIT);
            param.iData=0;
        }
    }
        break;
    default:
        break;
    }
}

void ER_ActionScript_Aya::Skill_RExit(tFSMData& param)
{
}

void ER_ActionScript_Aya::DeadEnter(tFSMData& param)
{
    GetOwner()->Animator3D()->SelectAnimation(L"Aya_Death", false);
}

void ER_ActionScript_Aya::DeadUpdate(tFSMData& param)
{
}

void ER_ActionScript_Aya::DeadExit(tFSMData& param)
{
}
