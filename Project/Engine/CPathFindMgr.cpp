#include "pch.h"
#include "CPathFindMgr.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLevel.h"
#include "CEventMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

CPathFindMgr::CPathFindMgr()
	: m_NavMesh(nullptr)
	, m_NavQuery(nullptr)
	, RayResultTrigger(false)
	, m_MapCollider(nullptr)
{
}

CPathFindMgr::~CPathFindMgr()
{
	if (m_NavMesh)
	{
		dtFreeNavMesh(m_NavMesh);
		m_NavMesh = nullptr;
	}
	if (m_NavQuery)
	{
		dtFreeNavMeshQuery(m_NavQuery);
		m_NavQuery = nullptr;
	}
}

void CPathFindMgr::init()
{
	if(!m_NavMesh && !m_NavQuery)
		LoadNavMeshFromFile("navmesh\\solo_navmesh_map.bin");
	
	
}

void CPathFindMgr::tick()
{

}

void CPathFindMgr::render()
{
}


bool CPathFindMgr::LoadNavMeshFromFile(const char* path)
{
	// NavMesh ���� ��ηκ��� �ҷ�����
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	wstring wpath = converter.from_bytes(path);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += wpath;

	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
	if (err != 0 || !fp)
	{
		return 0;
	}

	// NavMesh ��� �о����
	NavMeshSetHeader header;
	fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return false;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return false;
	}

	// NavMesh �ʱ�ȭ
	m_NavMesh = dtAllocNavMesh();
	if (!m_NavMesh)
	{
		fclose(fp);
		return false;
	}

	dtStatus status = m_NavMesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return false;
	}

	// NavMesh�� Ÿ�� ���� �о���� (����� Ÿ�� �ɼ��� ������� �ʽ��ϴ�)
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		fread(data, tileHeader.dataSize, 1, fp);

		m_NavMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	// NavQuery �ʱ�ȭ
	m_NavQuery = dtAllocNavMeshQuery();
	if (!m_NavQuery)
	{
		dtFreeNavMesh(m_NavMesh);
		m_NavQuery = nullptr;
		fclose(fp);
		return false;
	}
	if (dtStatusFailed(m_NavQuery->init(m_NavMesh, 2048))) {
		assert(false);
	}

	return true;
}

vector<Vec3> CPathFindMgr::FindPath(const Vec3& startPos, const Vec3& endPos)
{
	// ���� ��ġ�� �� ��ġ�� �����մϴ�.
	float startpos[3] = { startPos.x, startPos.y, -startPos.z }; // ���� ��ġ
	float endpos[3] = { endPos.x, endPos.y, -endPos.z }; // �� ��ġ

	dtPolyRef startRef, endRef;
	float polyPickExt[3] = { 6000,6000,6000 }; // ������ �����ϱ� ���� ����
	//float polyPickExt[3] = { 22000,22000,22000 }; // ������ �����ϱ� ���� ����

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF); // ��� ������ ����
	filter.setExcludeFlags(0);      // ������ ������ ����

	// ���� ������(��, ��, �� ��)�� ����ġ�� �ִ� �κ��ε� ���� �޽����� ���� ���� ������ �Ǿ����� �ʽ��ϴ�.
	//filter.setAreaCost(1, 1.0f); // Set cost for ground area.
	//filter.setAreaCost(2, 10.0f); // Set high cost for water area.
	//filter.setAreaCost(3, FLT_MAX); // Set infinite cost for wall area.

	// ����� ������ �˻�
	dtStatus status01 = m_NavQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
	dtStatus status02 = m_NavQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

	// ���۰� �� ��ġ�� ã���ϴ�.
	float nearestStartPos[3], nearestEndPos[3];
	status01 = m_NavQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
	status02 = m_NavQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

	// ��θ� ��ȹ�մϴ�.
	dtPolyRef path[MAX_POLY];
	int pathCount;
	m_NavQuery->findPath(startRef, endRef, nearestStartPos, nearestEndPos, &filter, path, &pathCount, 256);

	// ��θ� ���� ���� �̵� ��θ� �����մϴ�.
	float* actualPath = new float[3 * MAX_POLY];
	int actualPathCount;
	m_NavQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, &actualPathCount, MAX_POLY);

	// Vec3 ������ ��θ� �����մϴ�.
	vector<Vec3> vecPath(actualPathCount);
	for (int i = 0; i < actualPathCount; ++i)
	{
		vecPath[i] = Vec3(actualPath[3 * i], actualPath[3 * i + 1], -actualPath[3 * i + 2]);
	}

	delete[] actualPath; // ���̻� �ʿ���� calcPath�� �����մϴ�.

	return vecPath;
}

vector<CGameObject*> CPathFindMgr::CheckCollisionObject(IntersectResult _intersectResult)
{
	vector<CGameObject*> semiResult;
	vector<CGameObject*> result;

	for (UINT i = 0; i < m_vColliderObjects.size(); ++i) {
		Vec3 objTransform = m_vColliderObjects[i]->Transform()->GetRelativePos();
		Vec3 objScale = m_vColliderObjects[i]->Transform()->GetRelativeScale();

		if (m_vColliderObjects[i]->Collider2D()) {
			objTransform += m_vColliderObjects[i]->Collider2D()->GetOffsetPos();
			Vec3 colScale = m_vColliderObjects[i]->Collider2D()->GetOffsetScale();
			objScale.x += colScale.x;
			objScale.y += colScale.z;
			objScale.z += colScale.y;
		}
		if (m_vColliderObjects[i]->Collider3D()) {
			objTransform += m_vColliderObjects[i]->Collider3D()->GetOffsetPos();
			objScale += m_vColliderObjects[i]->Collider3D()->GetOffsetScale();
		}
		objScale *= 2;

		//ž����̶� y���� ��� ����.
		objTransform.y = 0.f;
		_intersectResult.vCrossPoint.y = 0.f;

		Vec3 objRange[2];
		objRange[0] = Vec3(objTransform.x - (objScale.x / 2), objTransform.y - (objScale.y / 2), objTransform.z - (objScale.z / 2));
		objRange[1] = Vec3(objTransform.x + (objScale.x / 2), objTransform.y + (objScale.y / 2), objTransform.z + (objScale.z / 2));
		
		if (objRange[0].x< _intersectResult.vCrossPoint.x && objRange[1].x > _intersectResult.vCrossPoint.x
			&& objRange[0].y< _intersectResult.vCrossPoint.y && objRange[1].y > _intersectResult.vCrossPoint.y
			&& objRange[0].z< _intersectResult.vCrossPoint.z && objRange[1].z > _intersectResult.vCrossPoint.z) {
			semiResult.push_back(m_vColliderObjects[i]);
		}
	}

	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();
	IntersectResult rayResult;
	for (UINT i = 0; i < semiResult.size(); ++i) {
		if (semiResult[i]->Collider2D()) {			
			rayResult = mainCam->IsCollidingBtwRayRect(ray, semiResult[i]);
		}
		if (semiResult[i]->Collider3D()) {
			switch (semiResult[i]->Collider3D()->GetColliderShape())
			{
			case COLLIDER3D_TYPE::CUBE:
				rayResult = mainCam->IsCollidingBtwRayCube(ray, semiResult[i]);
				break;
			case COLLIDER3D_TYPE::SPHERE:
				rayResult = mainCam->IsCollidingBtwRaySphere(ray, semiResult[i]);
				break;
			}
		}

		if (rayResult.bResult) {
			if (semiResult[i]->Collider2D())
				semiResult[i]->Collider2D()->OnRayOverlap();
			if (semiResult[i]->Collider3D())
				semiResult[i]->Collider3D()->OnRayOverlap();
			result.push_back(semiResult[i]);
		}
	}

	return result;
}
