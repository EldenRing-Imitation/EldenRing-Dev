#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"

#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CKeyMgr.h"
#include "CInstancingBuffer.h"
#include "CEngine.h"
#include "CCollider3D.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Frustum(this)
	, m_fAspectRatio(1.f)
	, m_fScale(1.f)
	, m_Near(1.f)
	, m_Far(10000.f)
	, m_FOV(XM_PI / 2.f)
	, m_OrthoWidth(0.f)
	, m_OrthoHeight(0.f)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_iLayerMask(0)
	, m_iLayerFrustum(0)
	, m_iCamIdx(-1)
	, m_bMainCamera(false)
	, m_bDebugView(true)
	, m_bDebugFrustumView(true)
{
	SetName(L"Camera");

	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResol.x / vRenderResol.y;

	m_OrthoWidth = vRenderResol.x;
	m_OrthoHeight = vRenderResol.y;
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Frustum(this)
	, m_fAspectRatio(_Other.m_fAspectRatio)
	, m_fScale(_Other.m_fScale)
	, m_Near(_Other.m_Near)
	, m_Far(_Other.m_Far)
	, m_FOV(_Other.m_FOV)
	, m_OrthoWidth(_Other.m_OrthoWidth)
	, m_OrthoHeight(_Other.m_OrthoHeight)
	, m_ProjType(_Other.m_ProjType)
	, m_iLayerMask(_Other.m_iLayerMask)
	, m_iLayerFrustum(_Other.m_iLayerFrustum)
	, m_iCamIdx(-1)
	, m_bMainCamera(false)
	, m_bDebugView(_Other.m_bDebugView)
	, m_bDebugFrustumView(_Other.m_bDebugFrustumView)
{
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
	if (-1 != m_iCamIdx)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
	}
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();

	m_Frustum.finaltick();

	// ���콺���� ���� ���
	CalRay();
}

tRay CCamera::CalRay()
{
	//// ���콺 ������ ���ϴ� Ray ���ϱ�
	//// SwapChain Ÿ���� ViewPort ����
	//CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	//D3D11_VIEWPORT tVP = pMRT->GetViewPort();
	//
	////  ���� ���콺 ��ǥ
	//Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	//
	//// ������ ī�޶��� ��ǥ�� �ݵ�� ������.
	//m_ray.vStart = Transform()->GetWorldPos();
	//
	//// view space ������ ����
	//m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	//m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	//m_ray.vDir.z = 1.f;
	//
	//// world space ������ ����
	//m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	//m_ray.vDir.Normalize();
	//
	//return m_ray;

	// ���콺 ������ ���ϴ� Ray ���ϱ�
	// SwapChain Ÿ���� ViewPort ����
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  ���� ���콺 ��ǥ
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
		// ������ ī�޶��� ��ǥ�� �ݵ�� ������.
		m_ray.vStart = Transform()->GetWorldPos();

		// view space ������ ����
		m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
		m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
		m_ray.vDir.z = 1.f;

		// world space ������ ����
		m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
		m_ray.vDir.Normalize();
	}

	else if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
	{
		Vec2 CamPos = CEngine::GetInst()->GetWindowResolution();
		CamPos.x = CamPos.x / 2.f;
		CamPos.y = CamPos.y / 2.f;

		Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos() - CamPos;

		Vec3 Addx = MousePos.x * Transform()->GetWorldDir(DIR_TYPE::RIGHT) * (1.f / m_fScale);
		Vec3 Addy = MousePos.y * Transform()->GetWorldDir(DIR_TYPE::UP) * (1.f / m_fScale);

		// Add�տ� -�� �ٿ��� �ϴ� ������ DirectX�� y��� ȭ�鿡���� y���� ���� ������
		// �ݴ��̱� �����̴�.
		m_ray.vStart = Transform()->GetWorldPos() + Addx - Addy;
		m_ray.vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	}
	return m_ray;
}

void CCamera::CalcViewMat()
{
	// ==============
	// View ��� ���
	// ==============
	m_matView = XMMatrixIdentity();

	// ī�޶� ��ǥ�� �������� �̵�
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// ī�޶� �ٶ󺸴� ������ Z ��� �����ϰ� ����� ȸ�� ����� ����
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;


	// View ����� ���ϱ�
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalcProjMat()
{
	// =============
	// ���� ��� ���
	// =============
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// ���� ����
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(m_OrthoWidth * (1.f / m_fScale), m_OrthoHeight * (1.f / m_fScale), m_Near, m_Far);
	}
	else
	{
		// ���� ����
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_fAspectRatio, m_Near, m_Far);
	}

	// ������� ����� ���ϱ�
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);
}

void CCamera::UpdateMatrix()
{
	// ��� ������Ʈ
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;

	g_transform.matProj = m_matProj;
	g_transform.matProjInv = m_matProjInv;
}


void CCamera::SetLayerMask(int _iLayer, bool _Visible)
{
	if (_Visible)
	{
		m_iLayerMask |= 1 << _iLayer;
	}
	else
	{
		m_iLayerMask &= ~(1 << _iLayer);
	}
}

void CCamera::SetLayerMaskAll(bool _Visible)
{
	if (_Visible)
		m_iLayerMask = 0xffffffff;
	else
		m_iLayerMask = 0;
}

void CCamera::SetLayerFrustum(int _iLayer, bool _Visible)
{
	if (_Visible)
	{
		m_iLayerFrustum |= 1 << _iLayer;
	}
	else
	{
		m_iLayerFrustum &= ~(1 << _iLayer);
	}
}

void CCamera::SetLayerFrustumAll(bool _Visible)
{
	if (_Visible)
		m_iLayerFrustum = 0xffffffff;
	else
		m_iLayerFrustum = 0;
}

void CCamera::SetCameraIndex(int _idx)
{
	if (_idx == 0)
		m_bMainCamera = true;
	else
		m_bMainCamera = false;

	m_iCamIdx = _idx;
	CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
}

void CCamera::MatrixUpdate()
{
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;

	g_transform.matProj = m_matProj;
	g_transform.matProjInv = m_matProjInv;
}

tRaycastOutV3 CCamera::Raycasting(Vec3* _vertices, tRay _ray)
{
	tRaycastOutV3 result;
	result.vUV = Vec3(0.f, 0.f, 0.f);
	result.bSuccess = false;

	Vec3 edge[2] = { Vec3(), Vec3() };
	edge[0] = _vertices[1] - _vertices[0];
	edge[1] = _vertices[2] - _vertices[0];

	Vec3 normal = (edge[0].Cross(edge[1])).Normalize();
	float b = normal.Dot(_ray.vDir);


	Vec3 w0 = _ray.vStart - _vertices[0];
	float a = -(normal.Dot(w0));
	float t = a / b;

	result.fDist = t;

	Vec3 p = _ray.vStart + t * _ray.vDir;

	result.vUV = p;

	float uu, uv, vv, wu, wv, inverseD;
	uu = edge[0].Dot(edge[0]);
	uv = edge[0].Dot(edge[1]);
	vv = edge[1].Dot(edge[1]);

	Vec3 w = p - _vertices[0];
	wu = w.Dot(edge[0]);
	wv = w.Dot(edge[1]);

	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f)
	{
		result.vUV = Vec3();
		result.fDist = 0.0f;
		result.bSuccess = false;
		return result;
	}

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || v > 1.0f)
	{
		result.vUV = Vec3();
		result.fDist = 0.0f;
		result.bSuccess = false;
		return result;
	}

	result.bSuccess = true;
	return result;
}

void CCamera::SortObject()
{
	// ���� ������ �з����� ����
	clear();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� ����ũ Ȯ��
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// ������ ����� ���� ������Ʈ�� ����
				if (nullptr == pRenderCom)
					continue;

				//���ӿ�����Ʈ�� ��Ȱ��ȭ ���¶��?
				if (vecObject[j]->IsEnable() == false)
					continue;

				// Frustum Check
				// if (m_iLayerFrustum & (1 << i) || pRenderCom->IsFrustumCheck())	// ?�두체렌???��? ?�단, ?�속 ?�이???��? 본인 ?�체 ?�두체렌??체크가 ?�어?�는 경우
				// {
				// 	Vec3 vPos = vecObject[j]->Transform()->GetWorldPos();
				// 	if (IsDebugFrustumView()) // && CLevelMgr::GetInst()->GetCurLevel()->GetState() != LEVEL_STATE::PLAY)
				// 		DrawDebugSphere(vecObject[j]->Transform()->GetWorldBoundingMat(), Vec4(0.8f, 0.8f, 0.f, 0.5f), 0.f, false);
				// 
				// 	if (false == m_Frustum.FrustumCheckBound(vPos, vecObject[j]->Transform()->GetBoundingRadius()))
				// 		continue;
				// }

				// ���׸��� ������ŭ �ݺ�
				UINT iMtrlCount = pRenderCom->GetMtrlCount();

				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// ������ ���ų�, ������ ���̴��� ������ �ȵ� ���
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// ���̴� �����ο� ���� �з�
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASK:
					{
						// Shader �� POV �� ���� �ν��Ͻ� �׷��� �з��Ѵ�.
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

						if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
						{
							pMap = &m_mapInstGroup_D;
						}
						else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
							|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASK)
						{
							pMap = &m_mapInstGroup_F;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID �� 0 �� ==> Mesh �� Material �� ���õ��� �ʾҴ�.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObject[j], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObject[j], iMtrl });
						}
					}
					break;
					case SHADER_DOMAIN::DOMAIN_DECAL:
						m_vecDecal.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
						m_vecTransparent.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
						m_vecPost.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_UI:
						m_vecUI.push_back(vecObject[j]);
						break;
					}
				}
			}
		}
	}
}

void CCamera::SortObject(CCamera* _MainCamera)
{
	// ���� ������ �з����� ����
	clear();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� ����ũ Ȯ��
		if (_MainCamera->m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// ������ ����� ���� ������Ʈ�� ����
				if (nullptr == pRenderCom)
					continue;

				if (vecObject[j]->IsEnable() == false)
					continue;

				// Frustum Check
				if (_MainCamera->m_iLayerFrustum & (1 << i) || pRenderCom->IsFrustumCheck())	// ����ü���� ���� �Ǵ�, �Ҽ� ���̾� Ȥ�� ���� ��ü ����ü���� üũ�� �Ǿ��ִ� ���
				{
					Vec3 vPos = vecObject[j]->Transform()->GetWorldPos();

					if (false == _MainCamera->m_Frustum.FrustumCheckBound(vPos, vecObject[j]->Transform()->GetBoundingRadius() / 2.f))
						continue;

					if (vecObject[j]->GetParent() == nullptr && IsDebugFrustumView()) // && CLevelMgr::GetInst()->GetCurLevel()->GetState() != LEVEL_STATE::PLAY)
						DrawDebugSphere(vecObject[j]->Transform()->GetWorldBoundingMat(), Vec4(0.8f, 0.8f, 0.f, 0.5f), 0.f, false);
				}

				// ���׸��� ������ŭ �ݺ�
				UINT iMtrlCount = pRenderCom->GetMtrlCount();

				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// ������ ���ų�, ������ ���̴��� ������ �ȵ� ���
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// ���̴� �����ο� ���� �з�
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASK:
					{
						// Shader �� POV �� ���� �ν��Ͻ� �׷��� �з��Ѵ�.
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

						if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
						{
							pMap = &m_mapInstGroup_D;
						}
						else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
							|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASK)
						{
							pMap = &m_mapInstGroup_F;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID �� 0 �� ==> Mesh �� Material �� ���õ��� �ʾҴ�.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObject[j], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObject[j], iMtrl });
						}
					}
					break;
					case SHADER_DOMAIN::DOMAIN_DECAL:
						m_vecDecal.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
						m_vecTransparent.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
						m_vecPost.push_back(vecObject[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_UI:
						m_vecUI.push_back(vecObject[j]);
						break;
					}
				}
			}
		}
	}
}

void CCamera::SortObject_Shadow()
{
	// ���� ������ �з����� ����
	clear_shadow();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� ����ũ Ȯ��
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// ������ ����� ���� ������Ʈ�� ����
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMaterial(0)
					|| nullptr == pRenderCom->GetMaterial(0)->GetShader())
				{
					continue;
				}

				if (vecObject[j]->IsEnable() == false)
					continue;

				m_vecShadow.push_back(vecObject[j]);
			}
		}
	}
}

void CCamera::render()
{
	if (m_bMainCamera)
	{
		// ���̴� �����ο� ���� ���������� �׸���
		// Deferred MRT �� ����
		// Deferred ��ü���� Deferred MRT �� �׸���
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet(true);
		render_deferred();

		// Light MRT �� ����
		// ��ü�鿡 ����� ������ �׸���
		// Deferred ��ü�� ���� �����Ű��
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet(false);

		const vector<CLight3D*>& vecLight3D = CRenderMgr::GetInst()->GetLight3D();
		for (size_t i = 0; i < vecLight3D.size(); ++i)
		{
			vecLight3D[i]->render();
		}

		// Deferred MRT �� �׸� ��ü�� Light MRT ����� ������ ���ļ�
		// �ٽ� SwapChain Ÿ������ ���� �׸���
		// SwapChain MRT �� ����
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
		static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeMtrl");

		static bool bSet = false;
		if (!bSet)
		{
			bSet = true;
			pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex"));
			pMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex"));
			pMtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularTargetTex"));
			pMtrl->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex"));
			pMtrl->SetTexParam(TEX_4, CResMgr::GetInst()->FindRes<CTexture>(L"ShadowTargetTex"));
			pMtrl->SetTexParam(TEX_5, CResMgr::GetInst()->FindRes<CTexture>(L"DataTargetTex"));
		}

		pMtrl->UpdateData();
		pRectMesh->render(0);
	}

	// Forward Rendering
	render_forward();
	render_decal();
	render_transparent();

	// PostProcess
	render_postprocess();

	// UI
	render_ui();
}

void CCamera::render_shadowmap()
{
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecShadow.size(); ++i)
	{
		m_vecShadow[i]->render_shadowmap();
	}
}


void CCamera::clear()
{
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();

	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecDecal.clear();
	m_vecTransparent.clear();
	m_vecPost.clear();
	m_vecUI.clear();
}

void CCamera::clear_shadow()
{
	m_vecShadow.clear();
}

void CCamera::render_deferred()
{
	UpdateMatrix();

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �̸��̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing ���� Ŭ����
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pObj->Animator3D()->GetBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_forward()
{
	UpdateMatrix();

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �̸��̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing ���� Ŭ����
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->UpdateData();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pObj->Animator3D()->GetBoneCount());
		}

		pMtrl->UpdateData_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->UpdateData();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_decal()
{
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPost.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPost[i]->render();
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->render();
	}
}

void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_Near, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerFrustum, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
	fwrite(&m_bMainCamera, sizeof(bool), 1, _File);
	fwrite(&m_OrthoWidth, sizeof(float), 1, _File);
	fwrite(&m_OrthoHeight, sizeof(float), 1, _File);

}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_Near, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iLayerFrustum, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
	fread(&m_bMainCamera, sizeof(bool), 1, _File);
	fread(&m_OrthoWidth, sizeof(float), 1, _File);
	fread(&m_OrthoHeight, sizeof(float), 1, _File);

}

bool CCamera::IsDebugFrustumView()
{
	return m_bDebugFrustumView;
}
