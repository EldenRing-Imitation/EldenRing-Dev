#include "pch.h"
#include "CFindPath.h"

#include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

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
	//if (KEY_PRESSED(KEY::LBTN))
	//{
	//	LaycastResultTrigger = true;
	//
	//	tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
	//	float yPos = CPathFindMgr::GetInst()->FindYPos(Vec2(naviResult.resultPos.x, naviResult.resultPos.z));
	//	naviResult.resultPos.y = yPos;
	//	Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
	//	GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
	//	
	//	int a = 0;
	//}
	//
	////���⿡ naviResult������ ����� �� ƽ ������ ���� ����.
	//if (LaycastResultTrigger) {
	//
	//	LaycastResultTrigger = false;
	//}

	if (KEY_PRESSED(KEY::LBTN)) {
		tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
		//GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
		FindPath(Vec3(naviResult.resultPos.x, naviResult.resultPos.y, naviResult.resultPos.z));
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
	}
	// ���� ��ΰ� ���ٸ� �� �� ���� ��ġ ��ȯ
	else
	{
		m_vNextPos = Vec3(NaN, NaN, NaN);
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
