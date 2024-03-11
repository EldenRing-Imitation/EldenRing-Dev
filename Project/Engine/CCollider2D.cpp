#include "pch.h"
#include "CCollider2D.h"

#include "CScript.h"
#include "components.h"


CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Shape(COLLIDER2D_TYPE::RECT)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
	, m_bIsCollidedFromRay(false)
	, m_bDrawCollision(true)
{
	SetName(L"Collider2D");
}

CCollider2D::CCollider2D(const CCollider2D& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_vOffsetRot(_other.m_vOffsetRot)
	, m_bAbsolute(_other.m_bAbsolute)
	, m_Shape(_other.m_Shape)
	, m_bDrawCollision(_other.m_bDrawCollision)
	, m_bFixed(_other.m_bFixed)
{

}

CCollider2D::~CCollider2D()
{
}


void CCollider2D::finaltick()
{
	// �浹 ȸ���� ������ ���
	assert(0 <= m_iCollisionCount);

	// ũ��, ȸ��, �̵� ��� ����
	m_matColliderScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColliderRot = XMMatrixRotationX(m_vOffsetRot.x) * XMMatrixRotationY(m_vOffsetRot.y) * XMMatrixRotationZ(m_vOffsetRot.z);

	m_matColliderPos = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	// ũ�� * ȸ�� * �̵�
	m_matCollider2D = m_matColliderScale * m_matColliderRot * m_matColliderPos;

	// �θ� (����) ��� ����
	const Matrix& matWorld = Transform()->GetWorldMat();

	if (m_bFixed)
	{
		// �θ��� ������ ���� �������� (ȸ�� ����� 3��° ��)
		Vec3 forward = Vec3(Transform()->GetWorldRotMat()._13, Transform()->GetWorldRotMat()._23, Transform()->GetWorldRotMat()._33);

		// y�� ȸ�� ���� ����
		float yRotation = atan2(forward.x, forward.z);

		// y�� ȸ�� ��� ����
		Matrix matParentYRotation = XMMatrixRotationY(yRotation);

		// ��ġ ���
		Matrix matTranslateToObjPosition = XMMatrixTranslationFromVector(Transform()->GetWorldPos());

		// �浹ü�� ������, ������ ȸ��, �θ��� y�� ȸ��, �׸��� ��ġ ����� ����
		m_matCollider2D = m_matColliderScale * m_matColliderRot * matParentYRotation * matTranslateToObjPosition;
	}
	else if (m_bAbsolute)
	{
		// �θ��� Scale�� ���ֱ�
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider2D = m_matCollider2D * matParentScaleInv * matWorld;
	}
	else
	{
		m_matCollider2D *= matWorld;

		// �浹ü scale update
		m_matColliderScale *= Transform()->GetWorldScaleMat();
		//m_matColliderPos *= Transform()->GetWorldPosMat();
	}

	// DebugShape ��û
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	if (m_bDrawCollision)
	{
		// �������� 0.f�� ������ GameObject�� ������� Collider�� �������.
		if (COLLIDER2D_TYPE::CIRCLE == m_Shape)
			DrawDebugCircle(m_matCollider2D, vColor, 0.f);
		else
			DrawDebugRect(m_matCollider2D, vColor, 0.f);
	}
}


void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	m_iCollisionCount += 1;

	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider2D::OnOverlap(CCollider2D* _Other)
{
	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	m_iCollisionCount -= 1;

	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider2D::BeginRayOverlap()
{
	int a = 30;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &a);
}

void CCollider2D::OnRayOverlap()
{
	int a = 20;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &a);
}

void CCollider2D::EndRayOverlap()
{
	int b = 0;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &b);
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Shape, sizeof(UINT), 1, _File);
	fwrite(&m_bDrawCollision, sizeof(bool), 1, _File);

}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetRot, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Shape, sizeof(UINT), 1, _File);
	fread(&m_bDrawCollision, sizeof(bool), 1, _File);
}

void CCollider2D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	wstring wOffsetPos = Vec3ToWString(m_vOffsetPos);
	wstring wOffsetScale = Vec3ToWString(m_vOffsetScale);
	int bAbsolute = m_bAbsolute;
	wstring wColliderType = ToWString(m_Shape);

	wstring query = L"INSERT INTO COLLIDER2D(GameObject_ID, Pos, Scale, Absolute, ColliderType) VALUES("
		+ std::to_wstring(_gameObjectID) + L","
		+ L"'" + wOffsetPos + L"',"
		+ L"'" + wOffsetScale + L"',"
		+ std::to_wstring(bAbsolute) + L","
		+ L"'" + wColliderType + L"');";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);
}

void CCollider2D::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	sqlite3_stmt* stmt;
	const char* szQuery = "SELECT Pos, Scale, Absolute, ColliderType FROM COLLIDER2D WHERE GameObject_ID = ?";

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _gameObjectID);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			std::string offsetPos(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
			std::string offsetScale(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
			int bAbsolute = sqlite3_column_int(stmt, 2);
			std::string colliderType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

			wstring wOffsetPos = ToWString(offsetPos);
			wstring wOffsetScale = ToWString(offsetScale);

			wstring wColliderType = ToWString(colliderType);

			// ���⼭ wOffsetPos, wOffsetScale�� Vec3���� ��ȯ�Ͽ� ��� ������ �Ҵ�
			m_vOffsetPos = WStringToVec3(wOffsetPos);
			m_vOffsetScale = WStringToVec3(wOffsetScale);
			m_bAbsolute = bAbsolute;

			// ColliderType�� ������ Ÿ������ ��ȯ�Ͽ� �Ҵ�
			m_Shape = ToCollider2DType(wColliderType);
		}
		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}
}
