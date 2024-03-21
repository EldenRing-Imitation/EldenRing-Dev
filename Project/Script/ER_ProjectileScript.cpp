#include "pch.h"
#include "ER_ProjectileScript.h"
#include "ER_ProjectilePool.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CPathFindMgr.h>
#include <Engine\CCollisionMgr.h>



#define PROJECTILE_LAYER 14

ER_ProjectileScript::ER_ProjectileScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PROJECTILESCRIPT)
	, m_pShooter(nullptr)
	, m_pTarget(nullptr)
	, m_vSpawnPos{}
	, m_vDir{}
	, m_fFrontDir()
	, m_ProjectileType()
	, m_vProjectileScale{}
	, m_DmgType()
	, m_SkillInst(nullptr)
	, m_SkiilFunc(nullptr)
	, m_iPoolIdx(-1)
{
}

ER_ProjectileScript::~ER_ProjectileScript()
{
}

void ER_ProjectileScript::SetMeshData(Ptr<CMesh> _pMesh, Ptr<CMaterial> _pMtrl)
{
	MeshRender()->SetMesh(_pMesh.Get());
	MeshRender()->SetMaterial(_pMtrl, 0);
}

void ER_ProjectileScript::Dead()
{
	EraseObject(GetOwner());

	// ������ƮǮ���� �����ε��� ����� false�� �����ϱ�
	ER_ProjectilePool::GetInst()->SetDead(m_iPoolIdx);

	// Data �ʱ�ȭ Ŭ����
	
	m_pShooter = nullptr;
	m_pTarget = nullptr;
	m_vSpawnPos = {};
	m_fFrontDir = 0.f;
	m_vDir = {};
	m_vProjectileScale = {};

	m_DmgType = eDmgType::END;
	m_ProjectileType = eProjectileType::END;
	
	m_fSpeed = 0.f;
	m_fLifetTime = 0.f;

	m_SkillInst = nullptr;
	m_SkiilFunc = nullptr;
}

float ER_ProjectileScript::SetRotationToTaret(Vec3 _OwnerPos, Vec3 _TargetPos)
{
	Vec3 vPos = _OwnerPos;
	Vec3 vTargetDir = (_TargetPos - vPos).Normalize();	// ���� ���� ����
	
	// ���� ���� ���
	float yRad = atan2(-DirectX::XMVectorGetX(vTargetDir),
		sqrt(DirectX::XMVectorGetY(vTargetDir) *
			DirectX::XMVectorGetY(vTargetDir) +
			DirectX::XMVectorGetZ(vTargetDir) *
			DirectX::XMVectorGetZ(vTargetDir)));

	// ���Ͱ� ����� ��� 180�� ȸ��
	if (vTargetDir.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}

void ER_ProjectileScript::Shot()
{
	// ������Ÿ�� ��ũ��Ʈ���� ���� ������� ����
	ER_ProjectilePool::GetInst()->SetUse(m_iPoolIdx);

	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	vRot.y = SetRotationToTaret(m_vSpawnPos, vTargetPos);
	Transform()->SetRelativeRot(vRot);

	// ���̾ ���� �߰�
	SpawnGameObject(GetOwner(), m_vSpawnPos, PROJECTILE_LAYER);
	GetOwner()->SetOutOfLayer(false);
}

void ER_ProjectileScript::ShotTarget(CGameObject* _Shooter, CGameObject* _Target, Vec3 _SpawnPos, eDmgType _DmgType, float _fSpeed)
{
	m_pShooter = _Shooter;
	m_pTarget = _Target;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = _DmgType;
	m_fSpeed = _fSpeed;
}

void ER_ProjectileScript::ShotDir(CGameObject* _Shooter, Vec3 _SpawnPos, Vec3 _Dir, eDmgType _DmgType, float _fSpeed, float _fLifeTime)
{
}

void ER_ProjectileScript::begin()
{
	
}

void ER_ProjectileScript::tick()
{
	if (GetOwner()->IsOutofLayer())
	{
		Dead();
		return;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();
	Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
	
	// Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::UP);	// �޽��� UP������ ���ʹ�����
	// vFrontDir *= 1.f * m_fFrontDir;

	/*
	switch (m_ProjectileType)
	{
	case ER_ProjectileScript::USELIFETIME:
	{
		m_fLifetTime -= DT;
		if (m_fLifetTime <= 0)
		{
			Dead();
			return;
		}

		break;
	}
	case ER_ProjectileScript::GUIDED:
	{
		// if (!m_pTarget || !m_pTarget->IsEnable())
		// {
		// 	Dead();
		// 	return;
		// }
		

		// Ÿ�� ����Ʈ
		break;
	}
	};*/

	// if (m_pTarget)
	vRot.y = SetRotationToTaret(m_vSpawnPos, vTargetPos);
	Transform()->SetRelativeRot(vRot);
	vPos += (vTargetPos - vPos).Normalize() * DT * m_fSpeed;
	
	vPos.y = m_vSpawnPos.y;

	Transform()->SetRelativePos(vPos);
	
}

void ER_ProjectileScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner() == m_pTarget)
	{
		GetOwner()->SetOutOfLayer(true);
	}
}

void ER_ProjectileScript::OnOverlap(CCollider3D* _Other)
{
}

void ER_ProjectileScript::EndOverlap(CCollider3D* _Other)
{
}


