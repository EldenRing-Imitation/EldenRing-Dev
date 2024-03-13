#include "pch.h"
#include "CFindPath.h"

#include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CLevel.h"
#include "CRenderMgr.h"
#include "CCamera.h"

CFindPath::CFindPath()
	:CComponent(COMPONENT_TYPE::FINDPATH)
	, LaycastResultTrigger(false)
	, m_vNextPos(Vec3(NaN, NaN, NaN))
{
}

CFindPath::~CFindPath()
{
}

void CFindPath::begin()
{
}

void CFindPath::tick()
{
}

void CFindPath::finaltick()
{
	if (KEY_TAP(KEY::RBTN) && !(KEY_TAP(KEY::LSHIFT))) {
		
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "MovePoint", "RBTN");

		CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();
		if (Map) {
			CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
			tRay ray = mainCam->GetRay();
			IntersectResult result = mainCam->IsCollidingBtwRayRect(ray, Map);
			Vec3 TargetPos = result.vCrossPoint;

			vector<CGameObject*> rayColObjects = CPathFindMgr::GetInst()->CheckCollisionObject(result);
			
			if(rayColObjects.size() == 0)
				FindPath(TargetPos);
			else {

			}
		}		
	}
	PathMove(50.0f, false);
}

void CFindPath::FindPath(Vec3 endPos)
{
	// �� ��θ� ã�� ������ ���ο� ��η� ��ü��
	float dist = (endPos - m_vPrevEndPos).Length();

	//if (dist > 10.f) //������ ���� �������� ���� ��û�� �������� ���̰� 10�ȼ��ۿ��ȵȴٸ� ��ã�⸦ �������� ���� : �Ź� ����Ѱ���������� ĳ���Ͱ� �������� ���� �����ϱ� ����!
	//{
	m_vecPath.clear();
	// ������Ʈ�� ���� ��ġ
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// ���� ��θ� �������� ��θ� �޾ƿ�
	m_vecPath = CPathFindMgr::GetInst()->FindPath(CurPos, endPos);
	m_iPathCount = m_vecPath.size();
	m_iCurPathIdx = 0;

	m_vNextPos = Vec3(m_vecPath[0].x, m_vecPath[0].y, m_vecPath[0].z);

	m_vPrevEndPos = endPos;
	//}
}

void CFindPath::FindNextPath()
{
	m_iCurPathIdx++;

	// ���� ��ΰ� �ִٸ� �� ��ġ�� ��ȯ�ϰ�, ��� �ε��� ������Ŵ
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
		
		string nextpos = u8"������ǥ : ";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].x) + ",";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].y) + ",";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].z);
		DEBUG_LOG_PROCESS(ToString(GetOwner()->GetName()).c_str(), "FindNextPath", nextpos.c_str());
	}
	// ���� ��ΰ� ���ٸ� �� �� ���� ��ġ ��ȯ
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
		DEBUG_LOG_PROCESS(ToString(GetOwner()->GetName()).c_str(), "FindNextPath", u8"����");
	}
}

bool CFindPath::PathMove(float _fSpeed, bool _IsRotation)
{
	Vec3 NextPos = m_vNextPos;

	if (isnan(NextPos.x))
		return true;

	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (NextPos - curPos).Normalize();

	float speed = _fSpeed;

	Vec3 newPos = curPos + (speed* Dir * DT);

	GetOwner()->Transform()->SetRelativePos(newPos);

	if ((newPos - NextPos).Length() < _fSpeed * DT) {
		FindNextPath();
	}
	return true;
}

void CFindPath::ClearPath()
{
	m_vNextPos = Vec3(NaN, NaN, NaN);
}

void CFindPath::SaveToLevelFile(FILE* _File)
{
}

void CFindPath::LoadFromLevelFile(FILE* _FILE)
{
}

void CFindPath::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CFindPath::LoadFromDB(int _gameObjectID)
{
}
