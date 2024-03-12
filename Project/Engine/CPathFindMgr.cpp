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

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

CPathFindMgr::CPathFindMgr()
	: m_NavMesh(nullptr)
	, m_NavQuery(nullptr)
	, RayResultTrigger(false)
	, m_pNaviMap(nullptr)
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
	
	// �ӽ� ������Ʈ ����
	 if (nullptr != m_pNaviMap)
	 	delete m_pNaviMap;
}

void CPathFindMgr::init()
{
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

	vector<Vtx> vVtx;
	vector<UINT> vIdx;

	// ���ؽ� ID�� �����ϱ� ���� �� (���ؽ� ��ġ -> ���� ���ؽ� ����Ʈ ���� �ε���)
	std::map<tuple<float,float,float>, int> vertexIdMap;
	int nextVertexId = 0;

	// Ÿ�ϰ� �������� ��ȸ
	for (int i = 0; i < m_NavMesh->getMaxTiles(); ++i) {
		const dtMeshTile* tile = m_NavMesh->getTileAt(i,0,0);
		if (!tile->header) continue;

		// �� Ÿ�� ���� ������ ��ȸ
		for (int j = 0; j < tile->header->polyCount; ++j) {
			const dtPoly* poly = &tile->polys[j];

			// �������� ���� ��踦 �����ϴ��� Ȯ�� (�ɼ�)
			if (poly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION) continue;

			// �� �������� ���ؽ� �ε��� ��ȸ
			for (int k = 0; k < 3; ++k) {
				// �������� ���ؽ� �ε����� ����Ͽ� ���� ���ؽ� ��ǥ�� Ÿ���� ���ؽ� �迭���� ã��
				const float* v = &tile->verts[poly->verts[k] * 3];
				
				tuple<float, float, float> vertexKey(v[0], v[1], v[2]);

				Vtx vtx1;
				vtx1.vPos = Vec3(v[0], v[1], v[2]);
				vtx1.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
				vtx1.vUV = Vec2(0.f, 0.f);
				vtx1.vNormal = Vec3(0.f, 1.f, 0.f);

				if (vertexIdMap.find(vertexKey) == vertexIdMap.end()) {
					vVtx.push_back(vtx1);
					vertexIdMap.insert(make_pair(vertexKey, nextVertexId));
					vIdx.push_back(nextVertexId);
					nextVertexId++;
				}
				else {
					vIdx.push_back(vertexIdMap.find(vertexKey)->second);
				}
			}
		}
	}

	CMesh* mesh = new CMesh(true);
	mesh->Create(vVtx.data(), (UINT)vVtx.size(), vIdx.data(), (UINT)vIdx.size());
	CResMgr::GetInst()->AddRes<CMesh>(L"NavMesh", mesh);

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
