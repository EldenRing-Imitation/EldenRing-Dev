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
	, m_iPathCount(0)
	, m_iCurPathIdx(0)
	, m_fPrevDir(0.f)
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
	////여기에 naviResult변수를 만들면 한 틱 느리게 값을 받음.
	//if (LaycastResultTrigger) {
	//
	//	LaycastResultTrigger = false;
	//}

	if (KEY_TAP(KEY::RBTN)) {
	//if (CPathFindMgr::GetInst()->GetRayResultTrigger()) {
		//tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
		//GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
		//FindPath(Vec3(naviResult.resultPos.x, naviResult.resultPos.y, naviResult.resultPos.z));
		//FindPath(Vec3(naviResult.resultPos.x, naviResult.resultPos.z, naviResult.resultPos.y));
		//CPathFindMgr::GetInst()->SetRayResultTrigger(false);
		
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "MovePoint", "RBTN");

		CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();
		CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = mainCam->GetRay();
		IntersectResult result = mainCam->IsCollidingBtwRayRect(ray, Map);
		Vec3 TargetPos = result.vCrossPoint;

		FindPath(TargetPos);
	}
	PathMove(5.0f, true);
}

void CFindPath::FindPath(Vec3 endPos)
{
	// 새 경로를 찾을 때마다 새로운 경로로 대체함
	float dist = (endPos - m_vPrevEndPos).Length();

	//if (dist > 10.f) //이전에 찍어둔 목적지와 현재 요청한 목적지의 차이가 10픽셀밖에안된다면 길찾기를 진행하지 않음 : 매번 비슷한곳을찍었을때 캐릭터가 덜덜떠는 현상 방지하기 위함!
	//{
	m_vecPath.clear();
	// 오브젝트의 현재 위치
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

	// 현재 경로를 시작으로 경로를 받아옴
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

	// 다음 경로가 있다면 그 위치를 반환하고, 경로 인덱스 증가시킴
	if (m_iCurPathIdx != -1 && m_iCurPathIdx < m_iPathCount)
	{
		
		m_vNextPos = Vec3(m_vecPath[m_iCurPathIdx].x, m_vecPath[m_iCurPathIdx].y, m_vecPath[m_iCurPathIdx].z);
		
		string nextpos = u8"다음좌표 : ";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].x) + ",";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].y) + ",";
		nextpos += std::to_string(m_vecPath[m_iCurPathIdx].z);
	}
	// 다음 경로가 없다면 갈 수 없는 위치 반환
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

	// SetDir
	float CurfrontDir = GetFrontDir(Dir);

	m_fPrevDir = CurfrontDir;
	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, CurfrontDir, 0.f));

	float speed = _fSpeed;
	Vec3 newPos = curPos + (speed * Dir * DT);
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

float CFindPath::GetFrontDir(Vec3 _Direction)
{
	// Cal Front Dir
	float yRad = atan2(-DirectX::XMVectorGetX(_Direction),
		sqrt(DirectX::XMVectorGetY(_Direction) *
			DirectX::XMVectorGetY(_Direction) +
			DirectX::XMVectorGetZ(_Direction) *
			DirectX::XMVectorGetZ(_Direction)));

	// 위를 향하는 경우 radian 구해주기
	if (_Direction.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return yRad;
}