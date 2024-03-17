#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Engine\CFindPath.h>

#include "ER_CamControllerScript.h"
#include "ER_ActionScript_Character.h"

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
	, m_pActionScript(nullptr)
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

	tFSMData data = {};

	// [ Mouse Control ]
	// �̵�
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
			data.v4Data = vTargetPoint;
			m_pActionScript->Move(data);
		}
	}
	else if (KEY_PRESSED(KEY::RBTN))
	{
		// �̵� ����
		data.v4Data = vTargetPoint;
		m_pActionScript->Move(data);
	}
	
	if (KEY_TAP(KEY::A))
	{
		// ����
		// ���� ���°� �ƴѰ�� ���ݸ�ɴ�����

		// ���ݸ�� ���¶��
	}


	if (KEY_TAP(KEY::Q))
	{
		m_pActionScript->Skill_Q(data);
	}
	if (KEY_TAP(KEY::W))
	{
		m_pActionScript->Skill_W(data);
	}
	if (KEY_TAP(KEY::E))
	{
		data.v4Data = vTargetPoint;
		m_pActionScript->Skill_E(data);
	}
	if (KEY_TAP(KEY::R))
	{
		m_pActionScript->Skill_R(data);
	}
	

	// ����
	if (KEY_TAP(KEY::S))
	{
		// ���� ��� �ʱ�ȭ
		GetOwner()->FindPath()->ClearPath();
	}
	
	// �޽�
	if (KEY_TAP(KEY::X))
	{
		m_pActionScript->Rest(data);
	}

	// UI

	if (KEY_TAP(KEY::ESC))
	{
		// ���Ӹ޴�
		// ����, ���� ��
	}
	
	if (KEY_TAP(KEY::TAB))
	{
		// ���� UI ����
	}



	// Camera Pixed this Obj
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->FollowPlayerCamera();
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
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

CGameObject* ER_PlayerScript::GetFocusObj()
{
	return nullptr;
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (_Point - vPos).Normalize();
	
	return Dir;
}
