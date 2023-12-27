#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)	
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)}	
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	m_matWorldScale = XMMatrixIdentity();
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	
	m_matWorld = m_matWorldScale * matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);
	}

	// �θ� ������Ʈ Ȯ��
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// ���� = ���ÿ��� * �θ�ũ�� �� * �θ� ����(ũ��/ȸ��/�̵�)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale = pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}
		

		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos	, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot	, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{	
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}

void CTransform::SaveToDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	wstring wRelativePos = Vec3ToWString(m_vRelativePos);
	wstring wRelativeScale = Vec3ToWString(m_vRelativeScale);
	wstring wRelativeRot = Vec3ToWString(m_vRelativeRot);
	int bAbsolute = m_bAbsolute;

	wstring query = L"INSERT INTO TRANSFORM(GameObject_ID, Pos, Scale, Rot, Absolute) VALUES("
		+ std::to_wstring(_gameObjectID) + L","
		+ L"'" + wRelativePos + L"',"
		+ L"'" + wRelativeScale + L"',"
		+ L"'" + wRelativeRot + L"',"
		+ std::to_wstring(bAbsolute) + L");";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);
}

void CTransform::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	sqlite3_stmt* stmt;
	const char* szQuery = "SELECT Pos, Scale, Rot, Absolute FROM TRANSFORM WHERE GameObject_ID = ?";

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const unsigned char* posPtr = sqlite3_column_text(stmt, 0);
			std::string sRelativePos = reinterpret_cast<const char*>(posPtr);

			const unsigned char* scalePtr = sqlite3_column_text(stmt, 1);
			std::string sRelativeScale = reinterpret_cast<const char*>(scalePtr);

			const unsigned char* rotPtr = sqlite3_column_text(stmt, 2);
			std::string sRelativeRot = reinterpret_cast<const char*>(rotPtr);

			wstring wRelativePos = ToWString(sRelativePos);
			wstring wRelativeScale = ToWString(sRelativeScale);
			wstring wRelativeRot = ToWString(sRelativeRot);
			int bAbsolute = sqlite3_column_int(stmt, 3);

			// ���⼭ wRelativePos, wRelativeScale, wRelativeRot�� Vec3���� ��ȯ�Ͽ� ��� ������ �Ҵ��Ͻʽÿ�
			m_vRelativePos = WStringToVec3(wRelativePos);
			m_vRelativeScale = WStringToVec3(wRelativeScale);
			m_vRelativeRot = WStringToVec3(wRelativeRot);
			m_bAbsolute = bAbsolute;
		}
		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}
}