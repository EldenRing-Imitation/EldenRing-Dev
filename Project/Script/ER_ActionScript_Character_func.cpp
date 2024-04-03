#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"

#include "ER_PlayerScript.h"

#include <Engine/CPathFindMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CCollisionMgr.h>

// [ Status Func ]
tIngame_Stats* ER_ActionScript_Character::GetStatus()
{
	return m_Data->GetStatus();
}
tStatus_Effect* ER_ActionScript_Character::GetStatusEffect()
{
	return m_Data->GetStatusEffect();
}
ER_DataScript_Character* ER_ActionScript_Character::GetCharacterData()
{
	return GetOwner()->GetScript<ER_DataScript_Character>();
}

bool ER_ActionScript_Character::IsSkillOn(SKILLIDX _idx)
{
	return m_Data->GetSkill((UINT)_idx)->IsAction;
}

// [ Transform Func ]
Vec3 ER_ActionScript_Character::GetFocusPoint()
{
	// �񱳿� Map Collider �ޱ�
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// �񱳿� Main Camera�� Ray �ޱ�
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// ������ ��ġ ���
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

Vec3 ER_ActionScript_Character::GetFocusDir()
{
	// �񱳿� Map Collider �ޱ�
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// �񱳿� Main Camera�� Ray �ޱ�
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// ������ ��ġ ���
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;
	Vec3 vPos = Transform()->GetRelativePos();
	
	TargetPos.y = 0.f;
	vPos.y = 0.f;

	Vec3 Dir = (TargetPos - vPos).Normalize();

	return Dir;
}

Vec3 ER_ActionScript_Character::GetClearPoint(const Vec3& vDir, float dist)
{
	Vec3 vPos	= GetOwner()->Transform()->GetRelativePos();
	vPos += GetOwner()->FindPath()->findMaxClearPoint(vDir, 0.f, dist);

	return vPos;
}
float ER_ActionScript_Character::GetClearDistance(const Vec3& vDir, float dist)
{
	float ClearDist = 0.f;
	ClearDist = GetOwner()->FindPath()->findMaxClearDistance(vDir, 0.f, dist);

	return ClearDist;
}
float ER_ActionScript_Character::GetClearDistanceByWall(const Vec3& vDir, float dist)
{
	float ClearDist = 0.f;
	ClearDist = GetOwner()->FindPath()->findMaxClearDistanceByWall(vDir, 0.f, dist);

	return ClearDist;
}
Vec3 ER_ActionScript_Character::SetRotationToTarget(const Vec3& vTarget)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vTargetDir = (vTarget - vPos).Normalize();	// ���� ���� ����

	// ���� ���� ���
	float yRad = atan2(-DirectX::XMVectorGetX(vTargetDir),
		sqrt(DirectX::XMVectorGetY(vTargetDir) *
			DirectX::XMVectorGetY(vTargetDir) +
			DirectX::XMVectorGetZ(vTargetDir) *
			DirectX::XMVectorGetZ(vTargetDir)));

	// ���Ͱ� ����� ��� 180�� ȸ��
	if (vTargetDir.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	Vec3 FinalDir(0.f, yRad, 0.f);
	
	GetOwner()->Transform()->SetRelativeRot(FinalDir);

	return FinalDir;
}
bool ER_ActionScript_Character::IsInRange(CGameObject* Target, float _fRange)
{
	// ��Ÿ��Ǵ�
	Vec3 TargetPos = Target->Transform()->GetRelativePos();
	Vec3 OwnerPos = GetOwner()->Transform()->GetRelativePos();
	TargetPos.y = 0;
	OwnerPos.y = 0;

	float fDist = Vec3::Distance(TargetPos, OwnerPos);

	if (fDist <= _fRange)
		return true;

	return false;
}

bool ER_ActionScript_Character::IsInRangeWithAngle(CGameObject* _Owner, CGameObject* _Target, float _fRange, float _degree)
{
	Vec3 vOwnerPos = _Owner->Transform()->GetRelativePos();
	Vec3 vTargetPos = _Target->Transform()->GetRelativePos();

	// �¿� �Ǵܸ��� ���� y�� 0���� ����
	vOwnerPos.y = 0.f;
	vTargetPos.y = 0.f;

	// [ 1�� �Ÿ� �Ǵ� ]
	float dist = Vec3::Distance(vOwnerPos, vTargetPos);

	if (_fRange < dist)
		return false;

	// [ 2�� ���� �Ǵ� ]
	// A�� �ٶ󺸴� ���⺤�Ϳ� B������ ������ ������ ����Ͽ� ����Ȯ��
	Vec3 vFront = _Owner->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vToTarget = (vTargetPos - vOwnerPos).Normalize();

	float dot = vFront.Dot(vToTarget);

	// ���⺤�Ͱ��� ���� ���
	float angle = acos(dot);

	// ������ ��ȣ�� Ȯ���Ͽ� ���� �� �������� ���ϴ� ������ ����
	float frontAngle = (vFront.Cross(vToTarget).y > 0) ? angle : -angle;

	// ������ 0������ 360�� ������ ��ȯ
	if (frontAngle < 0)
		frontAngle += 2 * XM_PI;

	float degree = (frontAngle * (180.f / XM_PI));
	
	// ���ʹ����� 0���� ����
	degree = fabsf(180.f - degree);

	return degree <= _degree;
}

Vec3 ER_ActionScript_Character::GetProjSpawnPos(DWORD_PTR _Target)
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vTargetPos = ((CGameObject*)_Target)->Transform()->GetRelativePos();
	vTargetPos.y = 0.f;
	
	Vec3 vDir = { vPos.x, 0.f, vPos.z };
	vDir = (vTargetPos - vDir).Normalize();

	vPos.y += 1.3f;
	vPos += vDir * 0.5f;

	return vPos;
}

// [ State Manage ]
void ER_ActionScript_Character::StateInit()
{
	StateList[(UINT)ER_CHAR_ACT::WAIT] = CreateWait();
	StateList[(UINT)ER_CHAR_ACT::MOVE] = CreateMove();
	StateList[(UINT)ER_CHAR_ACT::FARMING] = CreateFarming();
	StateList[(UINT)ER_CHAR_ACT::CRAFT] = CreateCraft();
	StateList[(UINT)ER_CHAR_ACT::REST] = CreateRest();
	StateList[(UINT)ER_CHAR_ACT::ATTACK] = CreateAttack();
	StateList[(UINT)ER_CHAR_ACT::ARRIVE] = CreateArrive();
	StateList[(UINT)ER_CHAR_ACT::DEAD] = CreateDead();
	StateList[(UINT)ER_CHAR_ACT::SKILL_Q] = CreateSkill_Q();
	StateList[(UINT)ER_CHAR_ACT::SKILL_W] = CreateSkill_W();
	StateList[(UINT)ER_CHAR_ACT::SKILL_E] = CreateSkill_E();
	StateList[(UINT)ER_CHAR_ACT::SKILL_R] = CreateSkill_R();

	if (StateList[(UINT)ER_CHAR_ACT::WAIT])
		StateList[(UINT)ER_CHAR_ACT::WAIT]->SetName(L"WAIT");
	if (StateList[(UINT)ER_CHAR_ACT::MOVE])
		StateList[(UINT)ER_CHAR_ACT::MOVE]->SetName(L"MOVE");
	if (StateList[(UINT)ER_CHAR_ACT::FARMING])
		StateList[(UINT)ER_CHAR_ACT::FARMING]->SetName(L"FARMING");
	if (StateList[(UINT)ER_CHAR_ACT::CRAFT])
		StateList[(UINT)ER_CHAR_ACT::CRAFT]->SetName(L"CRAFT");
	if (StateList[(UINT)ER_CHAR_ACT::REST])
		StateList[(UINT)ER_CHAR_ACT::REST]->SetName(L"REST");
	if (StateList[(UINT)ER_CHAR_ACT::ATTACK])
		StateList[(UINT)ER_CHAR_ACT::ATTACK]->SetName(L"ATTACK");
	if (StateList[(UINT)ER_CHAR_ACT::ARRIVE])
		StateList[(UINT)ER_CHAR_ACT::ARRIVE]->SetName(L"ARRIVE");
	if (StateList[(UINT)ER_CHAR_ACT::DEAD])
		StateList[(UINT)ER_CHAR_ACT::DEAD]->SetName(L"DEAD");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_Q])
		StateList[(UINT)ER_CHAR_ACT::SKILL_Q]->SetName(L"SKILL_Q");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_W])
		StateList[(UINT)ER_CHAR_ACT::SKILL_W]->SetName(L"SKILL_W");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_E])
		StateList[(UINT)ER_CHAR_ACT::SKILL_E]->SetName(L"SKILL_E");
	if (StateList[(UINT)ER_CHAR_ACT::SKILL_R])
		StateList[(UINT)ER_CHAR_ACT::SKILL_R]->SetName(L"SKILL_R");

	SoundLoad();
}
bool ER_ActionScript_Character::ChangeState(ER_CHAR_ACT _state, eAccessGrade _Grade)
{
	// �������ۻ��¿��� & �������
	if (m_iCurState == (UINT)_state || m_Data->IsDeadState())
		return false;

	// ���� ���� ���� �˻�
	if (IsAbleChange(_Grade))
	{
		m_iPrevState = m_iCurState;
		m_iCurState = (UINT)_state;
		FSMContext->ChangeState(StateList[(UINT)_state]);
	}

	return true;
	// TO_DO : ���� ��ȯ�� �Ұ����Ѱ�� ��Ƴ���.
}
bool ER_ActionScript_Character::IsAbleChange(eAccessGrade _Grade)
{
	// ���ڷ� ���� ���ۺ��氡�� ������
	// ���缳���� ���ۺ��氡�� ���ذ� ���ų� ���ƾ� true ��ȯ.

	return m_AccessGrade <= _Grade;
}


bool ER_ActionScript_Character::IsPlayer()
{
	ER_PlayerScript* IsPlayer = GetOwner()->GetScript<ER_PlayerScript>();
	
	return nullptr != IsPlayer;
}

bool ER_ActionScript_Character::IsCharacter(CGameObject* _Obj)
{
	ER_DataScript_Character* CharacterScript = _Obj->GetScript<ER_DataScript_Character>();
	
	return nullptr != CharacterScript;
}

bool ER_ActionScript_Character::IsDead(CGameObject* _Obj)
{
	ER_DataScript_Character* CharacterScript = _Obj->GetScript<ER_DataScript_Character>();
	
	return CharacterScript->IsDeadState();
}
