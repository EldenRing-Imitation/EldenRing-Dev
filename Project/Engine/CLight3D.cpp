#include "pch.h"
#include "CLight3D.h"

#include "CResMgr.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_bDebug(false)
{	
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
	SetLightColor(Vec3(1.f, 1.f, 1.f));
	SetLightAmbient(Vec3(0.f, 0.f, 0.f));
}

CLight3D::~CLight3D()
{

}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	
	m_LightIdx = (UINT)CRenderMgr::GetInst()->RegisterLight3D(this, m_LightInfo);

	UINT LightType = m_LightInfo.LightType;
	if (m_bDebug)
	{
		if (LightType == (UINT)LIGHT_TYPE::POINT)
		{
			DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 0.5f), 0.f, true);
		}
		else if (LightType == (UINT)LIGHT_TYPE::SPOT)
		{
			// �� �����
		}
	}
}

void CLight3D::render()
{
	Transform()->UpdateData();
	
	// Light ���� ������Ʈ
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);
	m_Mtrl->UpdateData();

	// ���� �޽� ����
	m_Mesh->render();
}


void CLight3D::SetRadius(float _Radius)
{
	m_LightInfo.Radius = _Radius;

	// SphereMesh �� ���� �������� 0.5f �̱� ������ 2��� ����
	Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}


void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// ������ ������ �� ��, ������ ��������� ����ȭ �� �� �ִ� �޽�(�����޽�) �� ����
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}

	else 
	{		
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}

	if (nullptr != m_Mtrl)
	{
		m_Mtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
		m_Mtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	}
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo.Color.vAmbient, sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.Color.vDiffuse, sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.vWorldPos,sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.vWorldDir,sizeof(Vec4), 1, _File);
	fwrite(&m_LightInfo.LightType,sizeof(UINT), 1, _File);
	fwrite(&m_LightInfo.Radius,sizeof(float), 1, _File);
	fwrite(&m_LightInfo.Angle,sizeof(float), 1, _File);
	fwrite(&m_LightInfo.padding, sizeof(int), 1, _File);
	fwrite(&m_LightIdx, sizeof(float), 1, _File);


}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo.Color.vAmbient, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.Color.vDiffuse, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.vWorldPos, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.vWorldDir, sizeof(Vec4), 1, _File);
	fread(&m_LightInfo.LightType, sizeof(UINT), 1, _File);
	fread(&m_LightInfo.Radius, sizeof(float), 1, _File);
	fread(&m_LightInfo.Angle, sizeof(float), 1, _File);
	fread(&m_LightInfo.padding, sizeof(int), 1, _File);
	fread(&m_LightIdx, sizeof(float), 1, _File);
}