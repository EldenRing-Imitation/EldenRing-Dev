#include "pch.h"
#include "CAnimEditObj.h"

#include <Engine\CResMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\CComponent.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CMRT.h>

#include <Engine\CTransform.h>
#include <Engine\CMeshRender.h>
#include <Engine\CCamera.h>

#include <Script\CCameraMoveScript.h>

CAnimEditObj::CAnimEditObj()
	: m_pRenderObj(nullptr)
	, m_pControlObj(nullptr)
	, m_pCamObj(nullptr)

{
}

CAnimEditObj::~CAnimEditObj()
{
	if (nullptr != m_pRenderObj)
		delete m_pRenderObj;
}

void CAnimEditObj::tick()
{
	if (nullptr != m_pRenderObj)
	{
		Vec3 vRot = m_pControlObj->Transform()->GetRelativeRot();
		Vec3 vScale = m_pControlObj->Transform()->GetRelativeScale();

		if (KEY_PRESSED(KEY::UP))
		{
			vRot.x += DT * 5.f;
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			vRot.x -= DT * 5.f;
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			vRot.y += DT * 5.f;
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			vRot.y -= DT * 5.f;
		}

		if (KEY_PRESSED(KEY::_1))
		{
			vScale.x += DT * 0.5f;
			vScale.y += DT * 0.5f;
			vScale.z += DT * 0.5f;
		}

		if (KEY_PRESSED(KEY::_2))
		{
			vScale.x -= DT * 0.5f;
			vScale.y -= DT * 0.5f;
			vScale.z -= DT * 0.5f;
		}

		m_pControlObj->Transform()->SetRelativeRot(vRot);
		m_pControlObj->Transform()->SetRelativeScale(vScale);
	}

}

void CAnimEditObj::setobject(Ptr<CMeshData> _MeshData)
{
	if (nullptr != m_pRenderObj)
		delete m_pRenderObj;

	m_pRenderObj = _MeshData->Instantiate();
	m_pRenderObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));
	m_pRenderObj->SetName(L"AnimEditObj");

	m_pRenderObj->AddComponent(new CAnimator3D);

	// ī�޶� ����
	m_pControlObj = new CGameObject;
	m_pControlObj->SetName(L"AnimEditControlObj");
	m_pControlObj->AddComponent(new CTransform);
	m_pControlObj->Transform()->SetRelativePos(Vec3(0.f, 1.f, 0.f));

	m_pCamObj = new CGameObject;
	m_pCamObj->SetName(L"AnimEditCamObj");
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -2.5f));

	m_pRenderObj->AddChild(m_pControlObj);
	m_pControlObj->AddChild(m_pCamObj);
}

void CAnimEditObj::update()
{
	if (nullptr != m_pRenderObj)
	{
		// tick
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != m_pRenderObj->GetComponent((COMPONENT_TYPE)i))
				m_pRenderObj->GetComponent((COMPONENT_TYPE)i)->tick();
		}

		// finaltick
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != m_pRenderObj->GetComponent((COMPONENT_TYPE)i))
				m_pRenderObj->GetComponent((COMPONENT_TYPE)i)->finaltick();
		}

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != m_pControlObj->GetComponent((COMPONENT_TYPE)i))
				m_pControlObj->GetComponent((COMPONENT_TYPE)i)->finaltick();
		}
		
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
		{
			if (nullptr != m_pCamObj->GetComponent((COMPONENT_TYPE)i))
				m_pCamObj->GetComponent((COMPONENT_TYPE)i)->finaltick();
		}
	}
}

void CAnimEditObj::render()
{
	if (nullptr != m_pRenderObj)
	{
		// Anim3D Editor�� ���̴��� ��ü
		ChangeEditShader(true);

		// Camera Matrix Update;
		CamMatrixUpdate();

		// Texture Render�� MRT ����
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::_3DANIM_EDIT)->OMSet();
		m_pRenderObj->render();

		// ���� ��/���� ��� ����
		g_transform.matView = m_matOriginView;
		g_transform.matProj = m_matOriginProj;

		// �⺻ ����Ÿ������ ����
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

		// ���� ���̴��� ��ü
		ChangeEditShader(false);
	}
}

void CAnimEditObj::Clear()
{

}

void CAnimEditObj::CamMatrixUpdate()
{
	// ���� ��� ����
	m_matOriginView = g_transform.matView;
	m_matOriginProj = g_transform.matProj;

	// �� ���
	{
		// ī�޶� ��� ���ϱ� 
		Matrix matView = XMMatrixIdentity();

		// ī�޶� ��ǥ�� �������� �̵�
		Vec3 vCamPos = m_pCamObj->Transform()->GetWorldPos();
		Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

		// ī�޶� �ٶ󺸴� ������ Z ��� �����ϰ� ����� ȸ�� ����� ����
		Matrix matViewRot = XMMatrixIdentity();

		Vec3 vR = m_pCamObj->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		Vec3 vU = m_pCamObj->Transform()->GetWorldDir(DIR_TYPE::UP);
		Vec3 vF = m_pCamObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);

		matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
		matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
		matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;
		matView = matViewTrans * matViewRot;

		g_transform.matView = matView;
	}

	// ���� ���
	{
		Matrix matProj = XMMatrixIdentity();
		
		// ���� ����
		matProj = XMMatrixPerspectiveFovLH(70.f, 1.f, 1.f, 10000.f);
		
		g_transform.matProj = matProj;
	}

}

void CAnimEditObj::ChangeEditShader(bool _SetType)
{
	if (_SetType)
	{
		UINT iSubsetCount = m_pRenderObj->MeshRender()->GetMesh()->GetSubsetCount();

		m_pOriginShader = m_pRenderObj->MeshRender()->GetMaterial(0)->GetShader();
		Ptr<CGraphicsShader> EditShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(L"AnimEdit3DShader");

		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != m_pRenderObj->MeshRender()->GetMaterial(i))
				m_pRenderObj->MeshRender()->GetMaterial(i)->SetShader(EditShader);
		}
	}

	else
	{
		UINT iSubsetCount = m_pRenderObj->MeshRender()->GetMesh()->GetSubsetCount();

		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != m_pRenderObj->MeshRender()->GetMaterial(i))
				m_pRenderObj->MeshRender()->GetMaterial(i)->SetShader(m_pOriginShader);
		}
	}
}
