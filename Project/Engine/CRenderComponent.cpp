#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_pSharedMtrl = _Mtrl;
	m_pCurrentMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	if (m_pCurrentMtrl != m_pSharedMtrl)
	{
		m_pCurrentMtrl = m_pSharedMtrl;
		m_pDynamicMtrl = nullptr;
	}

	return m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// ���� ������ ���� -> Nullptr ��ȯ
	if (nullptr == m_pSharedMtrl)
	{
		m_pCurrentMtrl = nullptr;
		return m_pCurrentMtrl;
	}

	// ���� ���� ���� ��û�� ���� �ؼ� �ش�.
	if (nullptr == m_pDynamicMtrl)
	{		
		m_pDynamicMtrl = m_pSharedMtrl->Clone();		
	}

	// ���� ������ ���� ��������� �����Ѵ�.
	m_pCurrentMtrl = m_pDynamicMtrl;


	return m_pCurrentMtrl;
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	SaveResRef(m_pMesh.Get(), _File);
	SaveResRef(m_pSharedMtrl.Get(), _File);	
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);
	LoadResRef(m_pSharedMtrl, _File);

	SetMaterial(m_pSharedMtrl);
}

void CRenderComponent::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// ���� ���ڿ� �غ�
	const char* szQuery = "INSERT INTO RENDERCOMPONENT(GameObject_ID, Mesh_Key, Mesh_Path, SharedMtrl_Key, SharedMtrl_Path) VALUES (?, ?, ?, ?, ?)";
	sqlite3_stmt* stmt;

	// ���� �غ�
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		wstring meshKey, meshPath, mtrlKey, mtrlPath;
		SaveResRefToDB(m_pMesh.Get(), meshKey, meshPath);
		SaveResRefToDB(m_pSharedMtrl.Get(), mtrlKey, mtrlPath);

		sqlite3_bind_text16(stmt, 2, meshKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 3, meshPath.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 4, mtrlKey.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text16(stmt, 5, mtrlPath.c_str(), -1, SQLITE_TRANSIENT);

		// ���� ����
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// ���� ó��: ���� ���࿡ �������� ���
			assert(false);
		}

		// ������Ʈ��Ʈ ����
		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}
}

void CRenderComponent::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	const char* szQuery = "SELECT Mesh_Key, Mesh_Path, SharedMtrl_Key, SharedMtrl_Path FROM RENDERCOMPONENT WHERE GameObject_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const wchar_t* meshKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 0));
			const wchar_t* meshPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* mtrlKey = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));
			const wchar_t* mtrlPath = static_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));

			LoadResRefFromDB(m_pMesh, meshKey, meshPath);
			LoadResRefFromDB(m_pSharedMtrl, mtrlKey, mtrlPath);

			SetMaterial(m_pSharedMtrl);
		}
		else {
			// ���� ó��: �����͸� ã�� ���߰ų� ������ �������� ���
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}
}
