#include "pch.h"
#include "ER_CamControllerScript.h"


#include <Engine\CRenderMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

#define DIR_FRONT	m_vDir[0]
#define DIR_UP		m_vDir[1]
#define DIR_RIGHT	m_vDir[2]


ER_CamControllerScript::ER_CamControllerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT)
	, m_pTargetObj(nullptr)
	, m_fCamSpeed(10.f)
	, m_bFollowToPlayer(false) // Player
	, m_fCurFOV(0.f)
{
}

ER_CamControllerScript::~ER_CamControllerScript()
{
}

void ER_CamControllerScript::begin()
{
	m_fCurFOV = Camera()->GetFOV();
}

void ER_CamControllerScript::tick()
{
	// �÷��̾� ������ ī�޶�
	if (m_bFollowToPlayer)
	{
		if (nullptr == m_pTargetObj)
		{
			m_bFollowToPlayer = false;
			return;
		}

		FollowPlayerCamera();
	}
	// ī�޶� ���� �̵�
	else
	{
		CameraMove();
	}

	float fCurFOV = Camera()->GetFOV();
	
	if (WHEEL_UP)
	{
		m_fCurFOV -= 0.0524f;
		m_fCurFOV = m_fCurFOV < 0.0872f ? 0.0872f : m_fCurFOV;
	}
	else if (WHEEL_DOWN)
	{
		m_fCurFOV += 0.0524f;
		m_fCurFOV = 0.698f < m_fCurFOV ? 0.698f : m_fCurFOV;
	}
	
	fCurFOV = AccelLerp(fCurFOV, 2.f);
	Camera()->SetFOV(fCurFOV);
}

void ER_CamControllerScript::SetTarget(CGameObject* _Target)
{
	m_pTargetObj = _Target;
	m_bFollowToPlayer = true;
}

void ER_CamControllerScript::CameraMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	vFront.y = 0.f;
	vRight.y = 0.f;

	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 10.f;

	if (KEY_PRESSED(KEY::UP)) { vPos += DT * vFront * fSpeed; };
	if (KEY_PRESSED(KEY::DOWN)) { vPos -= DT * vFront * fSpeed; };
	if (KEY_PRESSED(KEY::RIGHT)) { vPos += DT * vRight * fSpeed; };
	if (KEY_PRESSED(KEY::LEFT)) { vPos -= DT * vRight * fSpeed; };

	if (KEY_PRESSED(KEY::LCTRL) && KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * 5.f;
		vRot.x -= DT * vMouseDir.y * 5.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}

void ER_CamControllerScript::FollowPlayerCamera()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vTargetPos = m_pTargetObj->Transform()->GetRelativePos();
	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	Vec3 FinalPos = CalculateCamPos_ForTarget(vPos, vTargetPos, vFront);

	Transform()->SetRelativePos(FinalPos);
}

float ER_CamControllerScript::AccelLerp(float _CurFOV, float Acceleration)
{
	// ���ӵ� ���� ������ t �� ���
	float t = 0.1f;
	float adjustedT = 1.0f - pow(1.0f - t, Acceleration);

	// ������ ���� ��ȯ
	float res = _CurFOV + adjustedT * (m_fCurFOV - _CurFOV);
	
	return res;
}

Vec3 ER_CamControllerScript::CalculateCamPos_ForTarget(Vec3 _CamPos, Vec3 _vTargetPos, Vec3 _CamFrontDir)
{
	// Obj->Camera ����
	Vec3 vDir = _CamFrontDir * -1;
	
	Vec3 UpVector = { 0.f,1.f,0.f };
	float dotProduct = vDir.Dot(UpVector);

	float magnitude1 = vDir.Length();
	float magnitude2 = UpVector.Length();

	// �� ������ ������ ũ���� ������ ������ ���� ���
	float fradian = acos(dotProduct / (magnitude1 * magnitude2));

	float length = _CamPos.y / tan(fradian);

	// ī�޶� �߹��� �ƴ� ĳ���� �㸮�κ��� �Կ��ϵ��� ��������
	length -= 3.f;

	// Target To Camera �������� ������� Length��ŭ �̵����� �� ��ǥ
	vDir *= length;

	Vec3 Res = _vTargetPos + vDir;
	Res.y = _CamPos.y;
	
	return Res;
}