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
			// 콘 디버그
		}
	}
}

void CLight3D::render()
{
	Transform()->UpdateData();
	
	// Light 재질 업데이트
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);
	m_Mtrl->UpdateData();

	// 볼륨 메시 렌더
	m_Mesh->render();
}


void CLight3D::SetRadius(float _Radius)
{
	m_LightInfo.Radius = _Radius;

	// SphereMesh 의 로컬 반지름이 0.5f 이기 때문에 2배로 적용
	Transform()->SetRelativeScale(Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f));
}


void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		// 광원을 렌더링 할 때, 광원의 영향범위를 형상화 할 수 있는 메쉬(볼륨메쉬) 를 선택
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

void CLight3D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	const char* szQuery = "INSERT INTO LIGHT3D(GameObject_ID, LightInfo, Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path, Light_Idx) VALUES (?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);
		sqlite3_bind_blob(stmt, 2, &m_LightInfo, sizeof(m_LightInfo), SQLITE_STATIC);

		wstring meshKey, meshPath, mtrlKey, mtrlPath;
		SaveResRefToDB(m_Mesh.Get(), meshKey, meshPath);
		SaveResRefToDB(m_Mtrl.Get(), mtrlKey, mtrlPath);

		sqlite3_bind_text16(stmt, 3, meshKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 4, meshPath.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 5, mtrlKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 6, mtrlPath.c_str(), -1, SQLITE_TRANSIENT);

		sqlite3_bind_int(stmt, 7, m_LightIdx);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

void CLight3D::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT LightInfo, Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path, Light_Idx FROM LIGHT3D WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const void* data = sqlite3_column_blob(stmt, 0);
			int bytes = sqlite3_column_bytes(stmt, 0);

			// tLightInfo로 변환
			if (bytes == sizeof(tLightInfo)) {
				memcpy(&m_LightInfo, data, bytes);
			}
			else {
				assert(false);
			}

			// Mesh_Key, Mesh_Path, Mtrl_Key, Mtrl_Path 데이터 불러오기
			const wchar_t* meshKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* meshPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));
			const wchar_t* mtrlKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));
			const wchar_t* mtrlPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 4));

			LoadResRefFromDB(m_Mesh, meshKey, meshPath);
			LoadResRefFromDB(m_Mtrl, mtrlKey, mtrlPath);

			// Light_Idx 데이터 불러오기
			m_LightIdx = sqlite3_column_int(stmt, 5);
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