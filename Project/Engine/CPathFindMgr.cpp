#include "pch.h"
#include "CPathFindMgr.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CLevel.h"
#include "CEventMgr.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

CPathFindMgr::CPathFindMgr()
	: m_NavMesh(nullptr)
	, m_NavQuery(nullptr)
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
	LoadNavMeshFromFile("navmesh\\solo_navmesh.bin");
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
	m_NavQuery->init(m_NavMesh, 2048);

	return true;
}

vector<Vec3> CPathFindMgr::FindPath(const Vec3& startPos, const Vec3& endPos)
{
	// ���� ��ġ�� �� ��ġ�� �����մϴ�.
	float startpos[3] = { startPos.x, startPos.y, -startPos.z }; // ���� ��ġ
	float endpos[3] = { endPos.x, endPos.y, -endPos.z }; // �� ��ġ

	dtPolyRef startRef, endRef;
	float polyPickExt[3] = { 6000,6000,6000 }; // ������ �����ϱ� ���� ����

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF); // ��� ������ ����
	filter.setExcludeFlags(0);      // ������ ������ ����

	// ���� ������(��, ��, �� ��)�� ����ġ�� �ִ� �κ��ε� ���� �޽����� ���� ���� ������ �Ǿ����� �ʽ��ϴ�.
	//filter.setAreaCost(1, 1.0f); // Set cost for ground area.
	//filter.setAreaCost(2, 10.0f); // Set high cost for water area.
	//filter.setAreaCost(3, FLT_MAX); // Set infinite cost for wall area.

	// ����� ������ �˻�
	m_NavQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
	m_NavQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

	// ���۰� �� ��ġ�� ã���ϴ�.
	float nearestStartPos[3], nearestEndPos[3];
	m_NavQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
	m_NavQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

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

float CPathFindMgr::FindYPos(Vec2 _xzPos)
{
	int truncatedX = int(_xzPos.x / m_vNavMeshScale.x);
	int truncatedZ = int(_xzPos.y / m_vNavMeshScale.z);

	pair<int, int> fPair = make_pair(truncatedX, truncatedZ);

	int aArray[8] = { 1,0,-1,0,1,-1,1,-1 };
	int bArray[8] = {0,1,0,-1,-1,1,1,-1};

	auto findResult = m_mNaviMap.find(fPair);
	float zSum = 0;
	int count = 0;
	if (findResult == m_mNaviMap.end()) {
		for (UINT i = 0; i < 8; ++i) {
			int nextX = fPair.first + aArray[i];
			int nextY = fPair.second + bArray[i];

			findResult = m_mNaviMap.find(make_pair(nextX, nextY));
			if (findResult == m_mNaviMap.end())
				continue;
			zSum += findResult->second;
			count++;
		}

		m_mNaviMap.insert(make_pair(fPair, zSum / 8.f));

		return zSum / count * m_vNavMeshScale.y;
	}

	return findResult->second * m_vNavMeshScale.y;
}

void CPathFindMgr::SetNaviVtx(vector<Vec4> _naviVtx)
{
	m_vNaviVtx = _naviVtx;

	for (UINT i = 0; i < _naviVtx.size(); ++i) {
		int truncatedX = int(_naviVtx[i].x);
		int truncatedY = int(_naviVtx[i].y);
		pair<int, int> fPair= make_pair(truncatedX, truncatedY);
		m_mNaviMap.insert(make_pair(fPair, _naviVtx[i].z));
	}

	int a = 0;
}