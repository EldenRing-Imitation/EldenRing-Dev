#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>

#include "ER_CamControllerScript.h"
#include "ER_ActionScript_Character.h"

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
	, m_pActionScript(nullptr)
{
}

ER_PlayerScript::ER_PlayerScript(const ER_PlayerScript& _origin)
	: m_pActionScript(nullptr)
	, CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
{
}

ER_PlayerScript::~ER_PlayerScript()
{
}

void ER_PlayerScript::init()
{
}

void ER_PlayerScript::begin()
{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->SetTarget(GetOwner());

		m_pActionScript = GetOwner()->GetScript<ER_ActionScript_Character>();
}

void ER_PlayerScript::tick()
{
	// [ Mouse ]

	CGameObject* pFocusObj = GetFocusObj();	// Ÿ�� ������Ʈ
	Vec3 vTargetPoint = GetFocusPoint();	// Ÿ�� ����
	Vec3 vFocusDir = GetFocusDir(vTargetPoint);	// Ÿ�� ����

	if (KEY_TAP(KEY::RBTN))
	{
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "TAP", "RBTN");

		// ���� �浹���� ������Ʈ
		// Get Cur Collision CGameObject Type ��

		// cursor On UI
		if(false)
		{ }
		// cursor On Enemy
		else if(false)
		{ }
		// cursor On Box
		else if(false)
		{ }
		// cursor On Land (else)
		else
		{
			m_pActionScript->Move(pFocusObj, vTargetPoint);
		}
	}
	
	if (KEY_TAP(KEY::Q))
	{
		
	}
	if (KEY_TAP(KEY::W))
	{
	}
	if (KEY_TAP(KEY::E))
	{
	}
	if (KEY_TAP(KEY::R))
	{
	}
	
	if (KEY_TAP(KEY::A))
	{
	}

	// ����
	if (KEY_TAP(KEY::S))
	{
		// ���� ��� �ʱ�ȭ
		GetOwner()->FindPath()->ClearPath();
	}
	
	if (KEY_TAP(KEY::X))
	{
	}
	
	if (KEY_TAP(KEY::TAB))
	{
	}



	// Camera Pixed this Obj
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}
}

Vec3 ER_PlayerScript::GetFocusPoint()
{
	// �񱳿� Map Collider �ޱ�
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// �񱳿� Main Camera�� Ray �ޱ�
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// ������ ��ġ ���
	IntersectResult result = mainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

CGameObject* ER_PlayerScript::GetFocusObj()
{
	return nullptr;
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	// Cal Front Dir
	float yRad = atan2(-DirectX::XMVectorGetX(_Point),
		sqrt(DirectX::XMVectorGetY(_Point) *
			DirectX::XMVectorGetY(_Point) +
			DirectX::XMVectorGetZ(_Point) *
			DirectX::XMVectorGetZ(_Point)));

	// ���� ���ϴ� ��� radian �����ֱ�
	if (_Point.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return Vec3(0.f, yRad, 0.f);
}
