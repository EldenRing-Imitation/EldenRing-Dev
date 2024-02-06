#include "pch.h"
#include "CNaviMap.h"

#include "CResMgr.h"
#include "CComputeShader.h"
#include "CStructuredBuffer.h"

#include "CKeyMgr.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"

CNaviMap::CNaviMap()
	:CComponent(COMPONENT_TYPE::NAVIMAP)
{
	begin();
}

CNaviMap::~CNaviMap()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;
}

void CNaviMap::begin()
{
	// =====================
	// �����ɽ��� ���̴� ����
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastBaseShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader(32, 32, 1);
		m_pCSRaycast->CreateComputeShader(L"shader\\RaycastBase.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastBaseShader", m_pCSRaycast.Get());
	}

	//�����ɽ��� �� �ؽ��� �־��ֱ�.
	//Todo

	// ����ĳ���� ��� �޴� ����
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);
}

void CNaviMap::tick()
{
}

void CNaviMap::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();
	}
}

void CNaviMap::Raycasting()
{
	//NaviMap�� ���������Ϳ� �ε��������͸� ������
	Vtx* vertices = GetOwner()->GetRenderComponent()->GetMesh()->GetVtxSysMem();

	vector<Vector3> vVtx;

	for (auto& indexInfo : GetOwner()->GetRenderComponent()->GetMesh()->GetIdxInfo()) {
		UINT* indices = (UINT*)indexInfo.pIdxSysMem; // �ε��� ������ ����
		vVtx.resize(indexInfo.iIdxCount);
		for (UINT i = 0; i < indexInfo.iIdxCount; i += 3) {
			// �ﰢ�� ����
			Vtx v1 = vertices[indices[i]];
			Vtx v2 = vertices[indices[i + 1]];
			Vtx v3 = vertices[indices[i + 2]];
			vVtx[i] = v1.vPos;
			vVtx[i+1] = v1.vPos;
			vVtx[i+2] = v1.vPos;
		}
	}

	// ���� ī�޶� ������
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// ���� ���� ������ ������ ���÷� ����
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// ������ ī�޶� Ray �� ������ ����
	tRaycastOut out = { Vec2(0.f, 0.f), (float)0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetVtx(vVtx);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();

	m_pCrossBuffer->GetData(&out);
	int a = 0;
}

void CNaviMap::SaveToLevelFile(FILE* _File)
{
}

void CNaviMap::LoadFromLevelFile(FILE* _FILE)
{
}

void CNaviMap::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CNaviMap::LoadFromDB(int _gameObjectID)
{
}
