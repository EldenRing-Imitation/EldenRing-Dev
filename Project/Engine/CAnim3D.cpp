#include "pch.h"
#include "CAnim3D.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"

CAnim3D::CAnim3D()
	: m_pOwner(nullptr)
	, m_dCurTime(0)
	, m_tMTAnimClip{}
{
}

CAnim3D::CAnim3D(const CAnim3D& _origin)
	:m_pOwner(nullptr)
	, m_dCurTime(0)
	, m_tMTAnimClip(_origin.m_tMTAnimClip)
{
}

CAnim3D::~CAnim3D()
{
}

void CAnim3D::finaltick()
{
	m_dCurTime = 0.f;
	
	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_tMTAnimClip.fUpdateTime += DT;

	if (m_tMTAnimClip.fUpdateTime >= m_tMTAnimClip.dTimeLength)
	{
		m_tMTAnimClip.fUpdateTime = 0.f;
	}

	m_dCurTime = m_tMTAnimClip.dStartTime + m_tMTAnimClip.fUpdateTime;
	
	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * (double)m_pOwner->m_iFrameCount;
	m_pOwner->m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_pOwner->m_iFrameIdx >= m_tMTAnimClip.iFrameLength - 1)
		m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
		m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_pOwner->m_fRatio = (float)(dFrameIdx - (double)m_pOwner->m_iFrameIdx);

	// 컴퓨트 쉐이더 연산여부
	m_pOwner->m_bFinalMatUpdate = false;
}

/*
void CAnim3D::Create(const wstring& _strAnimName, int _iStartFrm, int _iEndFrm)
{
	m_strAnimName = _strAnimName;
	m_iStartFrame = _iStartFrm;
	m_iEndFrame = _iEndFrm;
	m_iFrameLength = _iEndFrm - _iStartFrm;
	m_dStartTime = _iStartFrm / m_iFrameCount;
	m_dEndTime = _iEndFrm / m_iFrameCount;
	m_dTimeLength = m_dEndTime - m_dStartTime;
	m_fUpdateTime = 0.f;
	m_dCurTime = 0.f;
}
*/