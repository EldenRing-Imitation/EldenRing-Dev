#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	if (KEY_PRESSED(KEY::C))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * (XM_PI / 2.f);
		Transform()->SetRelativeRot(vRot);
	}

	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	CRenderMgr::GetInst()->RegisterLight2D(m_LightInfo);
}

void CLight2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight2D::SaveToDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// ���� ���ڿ� �غ�
	const char* szQuery = "INSERT INTO LIGHT2D(GameObject_ID, LightInfo) VALUES (?, ?)";
	sqlite3_stmt* stmt;

	// ���� �غ�
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// GameObject_ID ���ε�
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		// m_LightInfo�� BLOB���� ���ε�
		sqlite3_bind_blob(stmt, 2, &m_LightInfo, sizeof(m_LightInfo), SQLITE_STATIC);

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

void CLight2D::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	sqlite3_stmt* stmt;
	const char* szQuery = "SELECT LightInfo FROM LIGHT2D WHERE GameObject_ID = ?";

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const void* data = sqlite3_column_blob(stmt, 0);
			int bytes = sqlite3_column_bytes(stmt, 0);

			// tLightInfo�� ��ȯ
			if (bytes == sizeof(tLightInfo)) {
				memcpy(&m_LightInfo, data, bytes);
			}
			else {
				assert(false);
			}
		}
		else {
			assert(false);
		}

		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}
}
