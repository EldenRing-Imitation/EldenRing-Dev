#include "pch.h"
#include "CEditCamControlScript.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

CEditCamControlScript::CEditCamControlScript()
	: CScript((UINT)SCRIPT_TYPE::EDITCAMCONTROLSCRIPT)
	, m_fCamSpeed(10.f)
	, b_RBTNPressed(false)
{
}

CEditCamControlScript::~CEditCamControlScript()
{
}

void CEditCamControlScript::tick()
{
	if (LEVEL_STATE::PLAY != CLevelMgr::GetInst()->GetCurLevel()->GetState())
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
			Camera2DMove();
		else
			Camera3DMove();
	}
}

void CEditCamControlScript::Camera2DMove()
{
	// Ű �Է¿� ���� ī�޶� �̵�
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fCamSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * fSpeed;
	}

	if (KEY_PRESSED(KEY::_1))
	{
		float fScale = Camera()->GetScale();
		fScale += DT * 1.f;
		Camera()->SetScale(fScale);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		float fScale = Camera()->GetScale();
		fScale -= DT * 1.f;
		Camera()->SetScale(fScale);
	}

	Transform()->SetRelativePos(vPos);
}

void CEditCamControlScript::Camera3DMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 10.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::E))
	{
		vPos += DT * vUp * fSpeed;
	}

	if (KEY_PRESSED(KEY::Q))
	{
		vPos -= DT * vUp * fSpeed;
	}



	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * 0.1f;
		vRot.x -= DT * vMouseDir.y * 0.1f;
		b_RBTNPressed = true;
	}
	else
		b_RBTNPressed = false;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);
}
