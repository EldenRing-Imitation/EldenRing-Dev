#include "pch.h"
#include "ER_DataScript_Projectile.h"

#include "ER_DataScript_Character.h"

#define PROJECTILE_LAYER 14

ER_DataScript_Projectile::ER_DataScript_Projectile(SCRIPT_TYPE _type)
	: CScript((UINT)_type)
    , m_pShooter(nullptr)
	, m_pTarget(nullptr)
	, m_vSpawnPos{}
	, m_vDir{}
	, m_DmgType(eDmgType::END)
	, m_SkillInst(nullptr)
	, m_SkillFunc(nullptr)
	, m_fSpeed(0.f)
	, m_fLifeTime(0.f)
	, m_SkillData(nullptr)
	, m_SkillHitStep(0)
{
}

ER_DataScript_Projectile::~ER_DataScript_Projectile()
{
}


void ER_DataScript_Projectile::init()
{
	AddComponents(GetOwner(), _TRANSFORM | _COLLIDER3D | _MESHRENDER);
}

float ER_DataScript_Projectile::SetRotationToTarget(Vec3 _OwnerPos, Vec3 _TargetPos)
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

void ER_DataScript_Projectile::begin()
{
}

void ER_DataScript_Projectile::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativePos();

	// Ÿ�� ���� �̵�
	if (m_pTarget)
	{
		Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
		vTargetPos.y = m_vSpawnPos.y;

		
		vPos += (vTargetPos - vPos).Normalize() * DT * m_fSpeed;
	}
	// ���� �̵�
	else
	{
		m_fLifeTime -= DT;

		if (m_fLifeTime <= 0)
		{
			m_fLifeTime = 0.f;
			DestroyObject(GetOwner());
			return;
		}

		vPos += m_vDir * DT * m_fSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void ER_DataScript_Projectile::Spawn()
{
	// ���� ����������� ȸ��
	Vec3 vRot = Transform()->GetRelativeRot();

	if (m_pTarget)
	{
		Vec3 vTargetPos = m_pTarget->Transform()->GetRelativePos();
		vTargetPos.y = 0.f;
		Vec3 vShooterPos = m_pShooter->Transform()->GetRelativePos();
		vShooterPos.y = 0.f;

		vRot.y = SetRotationToTarget(vShooterPos, vTargetPos);
	}
	else
	{
		Vec3 vTargetPos = m_vSpawnPos;
		vTargetPos.y = 0.f;
		Vec3 vShooterPos = m_pShooter->Transform()->GetRelativePos();
		vShooterPos.y = 0.f;

		m_vDir = (vTargetPos - vShooterPos).Normalize();

		vRot.y = SetRotationToTarget(vShooterPos, vTargetPos);
	}

	Transform()->SetRelativeRot(vRot);

	// ���̾ ���� �߰�
	SpawnGameObject(GetOwner(), m_vSpawnPos, PROJECTILE_LAYER);
}

void ER_DataScript_Projectile::SetForTarget(CGameObject* _Shooter, CGameObject* _Target, Vec3 _SpawnPos, float _fSpeed)
{
	m_pShooter = _Shooter;
	m_pTarget = _Target;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = eDmgType::NORMAL;
	m_fSpeed = _fSpeed;
}

void ER_DataScript_Projectile::SetForDir(CGameObject* _Shooter, Vec3 _SpawnPos, float _fSpeed, float _fLifeTime)
{
	m_pShooter = _Shooter;
	m_vSpawnPos = _SpawnPos;
	m_DmgType = eDmgType::NORMAL;
	m_fSpeed = _fSpeed;
	m_fLifeTime = _fLifeTime;
}

void ER_DataScript_Projectile::BeginOverlap(CCollider3D* _Other)
{
	// Ÿ�ٿ�����Ʈ�� �浹�� ������Ʈ�� ��ġ�Ѵ�.
	CGameObject* Target = _Other->GetOwner();
	
	if (m_pTarget && _Other->GetOwner() == m_pTarget)
	{
		// �����������Լ�
		BATTLE_COMMON(m_pShooter, m_pTarget);
		
		// ���� ���
		if (nullptr != m_pSound)
			m_pSound->Play(1, 0.5f, true);
		
		// ���̾�� ������������ ����
		DestroyObject(GetOwner());
	}
	// �߻��� ��ü�� �浹�Ѱ��� �����Ѵ�.
	
	if (!m_pTarget && _Other->GetOwner() != m_pShooter)
	{
		ER_DataScript_Character* CharacterScript = _Other->GetOwner()->GetScript<ER_DataScript_Character>();

		if (CharacterScript && !CharacterScript->IsDeadState())
		{
			// ������ Ÿ�Ժ��� �����Լ� 
			if (eDmgType::NORMAL == m_DmgType)
			{
				// �����������Լ�
				BATTLE_COMMON(m_pShooter, m_pTarget);

				// ���� ���
				if (nullptr != m_pSound)
					m_pSound->Play(1, 0.5f, true);

				DestroyObject(GetOwner());
			}
			else if (eDmgType::SKILL == m_DmgType)
			{
				if (m_SkillInst && m_SkillFunc)
				{
					ER_BattleSystem::GetInst()->Battle_Skill(m_pShooter, _Other->GetOwner(), m_SkillInst, m_SkillFunc, m_SkillData, m_SkillHitStep);

					// ���� ���
					if (nullptr != m_pSound)
						m_pSound->Play(1, 0.5f, true);

					DestroyObject(GetOwner());
				}
			}
		}
	}
}

void ER_DataScript_Projectile::OnOverlap(CCollider3D* _Other)
{
}

void ER_DataScript_Projectile::EndOverlap(CCollider3D* _Other)
{
}