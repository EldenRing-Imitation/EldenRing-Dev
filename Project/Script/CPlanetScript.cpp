#include "pch.h"
#include "CPlanetScript.h"

CPlanetScript::CPlanetScript()
	: CScript((UINT)SCRIPT_TYPE::PLANETSCRIPT)
	, m_pParent(nullptr)
	, m_vStarPos{}
	, m_bStar(false)
	, m_fOrbitSpd(0.f)
	, m_fRotSpd(0.f)
	, m_vOrbitAxis{ 0.f, 0.1f, 0.f }
	, m_vRotAxis{ 0.f, 0.1f, 0.f }
	, m_fRadius(0.f)
	, m_OrbitLineObj(nullptr)
{
}

CPlanetScript::~CPlanetScript()
{
}

void CPlanetScript::begin()
{
	// ������
	GetOwner()->Transform()->SetRelativeRot(m_vRotAxis);
	
	// if (nullptr != m_pParent)
	// {
	// 	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	// 	Vec3 ParentPos = m_pParent->Transform()->GetRelativePos();
	// 	vPos = vPos - ParentPos;
	// 	m_fRadius = vPos.Length();
	// 	m_vStarPos.y += 250.f;
	// }
}

void CPlanetScript::tick()
{
	// ����
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRot();
	vRot.y += m_fRotSpd * 0.5f * DT;

	if (2.f * XM_PI < vRot.y)
		vRot.y = vRot.y - 2.f * XM_PI;

	GetOwner()->Transform()->SetRelativeRot(vRot);
	
	// ����
	if (!m_bStar)
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

		// ���� ���ӵ� : Centripetal acceleration
		Vec3 vCentripetalAccel = GetOwner()->Transform()->GetRelativePos();
		vCentripetalAccel -= m_vStarPos;
		vCentripetalAccel.Normalize();

		// ���ӵ� : angular velocity a ���ɰ��ӵ� , b ��
		Vec3 vAcgularVelocity = XMVector3Cross(vCentripetalAccel, m_vOrbitAxis);
		vAcgularVelocity.Normalize();

		vAcgularVelocity *= m_fRadius;

		vAcgularVelocity += vCentripetalAccel;

		vPos += (vAcgularVelocity * DT) * m_fOrbitSpd;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}
}

void CPlanetScript::SaveToLevelFile(FILE* _File)
{
}

void CPlanetScript::LoadFromLevelFile(FILE* _File)
{
}

