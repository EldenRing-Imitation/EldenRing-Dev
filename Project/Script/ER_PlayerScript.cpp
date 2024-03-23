#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CUIMgr.h>

#include "ER_BattleSystem.h"
#include "ER_DataScript_Character.h"

#include <Engine\CFindPath.h>

#include "ER_CamControllerScript.h"
#include "ER_ActionScript_Character.h"

#include "CUIScript_Button.h"

#define LAYER_ITEMBOX 5
#define LAYER_MONSTER 11
#define LAYER_CHARACTER 12

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
	, m_Character(nullptr)
	, m_pActionScript(nullptr)
	, m_pRangeObject(nullptr)
	, m_AttackCsr(false)
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
	m_Character = GetOwner()->GetScript <ER_DataScript_Character>();
}

void ER_PlayerScript::tick()
{
	// ĳ���Ͱ� ������¸� ����
	if (m_Character->IsDeadState())
		return;
	
	// [ Mouse ]
	std::pair<CGameObject*,int> pTargetObj = GetFocusObj();	// Ÿ�� ������Ʈ
	Vec3 vCsrPoint = GetFocusPoint();	// Ÿ�� ����

	tFSMData data = {};

	// v4Data : ���콺 ��ġ, ���콺 ���� Ȱ�밡��
	// lParam : ���� ���콺��ġ�� �浹���� ������Ʈ

	data.v4Data = vCsrPoint;
	data.lParam = (DWORD_PTR)pTargetObj.first;

	// [ Mouse Control ]
	// ���콺�� UI���� �ִٸ� �Է��� ���� �ʴ´�.
	// �̵�
	if (KEY_TAP(KEY::RBTN))
	{
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "TAP", "RBTN");

		// ���� �浹���� ������Ʈ
		// Get Cur Collision CGameObject Type ��

		// ���ݴ����¶�� ����

		// cursor On UI
		CGameObject* FocusedUI = CUIMgr::GetInst()->GetFocusedUI();
		CGameObject* FocusedUIs = CUIMgr::GetInst()->GetFocusedPriorityUI();
		if (FocusedUI)
		{
			// UI�� UIMgr�� Ȯ���ϱ⶧���� UIMgr���� Focused UI�� �ִ��� �Ǵ�.
			// UI�� UI���� �Ǵ��ϱ⶧���� �������� �ʰ� �Ѿ��.

			// �÷��̾ũ��Ʈ������ ���� ���� ���� ����
			int a = 0;
		}
		else if (pTargetObj.second == LAYER_ITEMBOX)
		{
			//�����ΰ��(Layer �̸� : ItemBox)
			m_pActionScript->Farming(data);
		}
		else if (pTargetObj.second == LAYER_MONSTER)
		{
			//�����ΰ��(Layer �̸� : Monster)
			int i = 0;
		}
		else if (pTargetObj.second == LAYER_CHARACTER)
		{
			m_pActionScript->Attack(data);

			//�� ĳ������ ���(Layer �̸� : Character)
		}
		// cursor On Land (else)
		else
		{
			m_pActionScript->Move(data);
			
		}
		m_AttackCsr = false;
	}
	
	
	if (KEY_TAP(KEY::A))
	{
		m_AttackCsr = true;
		// Ÿ�ٿ�����Ʈ�� �����°��  ���콺 �������ֱ�� �̵� ����
		
		// ����
		// ���� ���°� �ƴѰ�� ���ݴ����¼���
		// ���ݴ����´� ���콺Ŀ�����游 ����
	}


	if (m_AttackCsr && KEY_TAP(KEY::LBTN))
	{
		// KEY_TAP �տ� ���ݸ�ɻ��� �Ǵ� ������ �� �߰��Ͽ� (true && KEY_TAP(KEY::LBTN)) ���� ���� ����
		// ���ݴ����� ����
		// pTargetObj�� Enemy�� ��� Attack ��� ����
		if (pTargetObj.second == LAYER_CHARACTER)
		{
			m_pActionScript->Attack(data);
		}
		m_AttackCsr = false;
	}


	// v4Data : ���콺 ��ġ, ���콺 ���� Ȱ�밡��
	// lParam : ���� ���콺��ġ�� �浹���� ������Ʈ
	
	if (KEY_TAP(KEY::Q))
	{
		m_pActionScript->Skill_Q(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::W))
	{
		m_pActionScript->Skill_W(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::E))
	{
		m_pActionScript->Skill_E(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::R))
	{
		m_pActionScript->Skill_R(data);
		m_AttackCsr = false;
	}
	

	// ����
	if (KEY_TAP(KEY::S))
	{
		// ���� ��� �ʱ�ȭ
		GetOwner()->FindPath()->ClearPath();
		m_AttackCsr = false;
	}
	
	// �޽�
	if (KEY_TAP(KEY::X))
	{
		m_pActionScript->Rest(data);
		m_AttackCsr = false;
	}

	// ����(CRAFT)
	if (KEY_TAP(KEY::APOSTROPHE)) {
		m_pActionScript->Craft(data);
		m_AttackCsr = false;
	}

	// UI
	if (KEY_TAP(KEY::ESC))
	{
		// ���Ӹ޴�
		// ����, ���� ��
		m_AttackCsr = false;
	}
	
	if (KEY_TAP(KEY::TAB))
	{
		// ���� UI ����
	}



	// ī�޶� �þ� ĳ���ͷ� ����
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}

	// SPACE �� �����µ��� ����ī�޶� ĳ���͸� �����Ѵ�.
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

std::pair<CGameObject*,int> ER_PlayerScript::GetFocusObj()
{
	// ���콺 ���̿� ������ ������Ʈ �Ǵ�
	vector<CGameObject*> rayColObjs = CCollisionMgr::GetInst()->GetCurRayColObjs();
	
	if (rayColObjs.size() == 1) 
		return make_pair(rayColObjs[0], rayColObjs[0]->GetLayerIndex());
	else if (rayColObjs.size() > 1)
		return make_pair(rayColObjs[0], rayColObjs[0]->GetLayerIndex());
	else
		return make_pair(nullptr, -1);
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (_Point - vPos).Normalize();
	
	return Dir;
}