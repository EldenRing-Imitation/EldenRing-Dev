#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CLight3D.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"

#include "CRenderMgr.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"


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
}

void CCamera::CalcViewMat()
{
	// ==============
	// View ?�렬 계산
	// ==============
	m_matView = XMMatrixIdentity();

	// 카메??좌표�??�점?�로 ?�동
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메?��? 바라보는 방향??Z 축과 ?�행?�게 만드???�전 ?�렬???�용
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;


	// View ??��??구하�?
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalcProjMat()
{
	// =============
	// ?�영 ?�렬 계산
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

void CCamera::SortObject()
{
	// ���� ������ �з����� ����
	clear();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ?�이??마스???�인
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
					continue;

				// Frustum Check
				if (m_iLayerFrustum & (1 << i) || pRenderCom->IsFrustumCheck())	// ?�두체렌???��? ?�단, ?�속 ?�이???��? 본인 ?�체 ?�두체렌??체크가 ?�어?�는 경우
				{
					Vec3 vPos = vecObject[j]->Transform()->GetWorldPos();
					if (IsDebugFrustumView()) // && CLevelMgr::GetInst()->GetCurLevel()->GetState() != LEVEL_STATE::PLAY)
						DrawDebugSphere(vecObject[j]->Transform()->GetWorldBoundingMat(), Vec4(0.8f, 0.8f, 0.f, 0.5f), 0.f, false);
				
					if (false == m_Frustum.FrustumCheckBound(vPos, vecObject[j]->Transform()->GetBoundingRadius()))
						continue;
				}

					// ���̴� �����ο� ���� �з�
				SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(0)->GetShader()->GetDomain();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
					m_vecDeferred.push_back(vecObject[j]);
					break;

				case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					m_vecDeferredDecal.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObject[j]);
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
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMaterial(0)
					|| nullptr == pRenderCom->GetMaterial(0)->GetShader())
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

				// ���̴� �����ο� ���� �з�
				SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(0)->GetShader()->GetDomain();
				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
					m_vecDeferred.push_back(vecObject[j]);
					break;

				case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
					m_vecDeferredDecal.push_back(vecObject[j]);
					break;

				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObject[j]);
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

				m_vecShadow.push_back(vecObject[j]);
			}
		}
	}
}

void CCamera::render()
{
	// ?�렬 ?�데?�트
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;

	g_transform.matProj = m_matProj;
	g_transform.matProjInv = m_matProjInv;

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
		}

		pMtrl->UpdateData();
		pRectMesh->render(0);
	}

	// Forward Rendering
	render_opaque();
	render_mask();
	render_decal();
	render_transparent();

	// PostProcess - ?�처�?
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
	m_vecDeferred.clear();
	m_vecDeferredDecal.clear();

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
	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		m_vecDeferred[i]->render();
	}

	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED_DECAL)->OMSet();

	for (size_t i = 0; i < m_vecDeferredDecal.size(); ++i)
	{
		m_vecDeferredDecal[i]->render();
	}
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}

void CCamera::render_mask()
{
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->render();
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
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
	fwrite(&m_bMainCamera, sizeof(bool), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
	fread(&m_bMainCamera, sizeof(bool), 1, _File);
}

void CCamera::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	const char* szQuery = "INSERT INTO CAMERA(GameObject_ID, AspectRatio, Scale, ProjType, LayerMask, CamIdx, bMainCamera) VALUES (?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);
		sqlite3_bind_double(stmt, 2, static_cast<double>(m_fAspectRatio));
		sqlite3_bind_double(stmt, 3, static_cast<double>(m_fScale));
		sqlite3_bind_int(stmt, 4, static_cast<int>(m_ProjType));
		sqlite3_bind_int(stmt, 5, static_cast<int>(m_iLayerMask));
		sqlite3_bind_int(stmt, 6, m_iCamIdx);
		sqlite3_bind_int(stmt, 7, static_cast<int>(m_bMainCamera));

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

void CCamera::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT AspectRatio, Scale, ProjType, LayerMask, CamIdx, bMainCamera FROM CAMERA WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			m_fAspectRatio = static_cast<float>(sqlite3_column_double(stmt, 0));
			m_fScale = static_cast<float>(sqlite3_column_double(stmt, 1));
			m_ProjType = (PROJ_TYPE)(static_cast<UINT>(sqlite3_column_int(stmt, 2)));
			m_iLayerMask = static_cast<UINT>(sqlite3_column_int(stmt, 3));
			m_iCamIdx = sqlite3_column_int(stmt, 4);
			m_bMainCamera = static_cast<bool>(sqlite3_column_int(stmt, 5)) != 0;
		}
		else {
			assert(false);
		}
		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

bool CCamera::IsDebugFrustumView()
{
	return m_bDebugFrustumView;
}
