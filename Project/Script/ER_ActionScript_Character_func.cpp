#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCollisionMgr.h>

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
