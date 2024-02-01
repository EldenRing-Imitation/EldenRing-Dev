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
	, m_bPlay(true)
{
}

CAnim3D::CAnim3D(const CAnim3D& _origin)
	:m_pOwner(nullptr)
	, m_dCurTime(0)
	, m_tMTAnimClip(_origin.m_tMTAnimClip)
	, m_bPlay(true)
{
}

CAnim3D::~CAnim3D()
{
}

void CAnim3D::finaltick()
{
	m_dCurTime = 0.f;
	
	if (m_bPlay)
	{
		// ���� ������� Clip �� �ð��� �����Ѵ�.
		m_tMTAnimClip.fUpdateTime += DT;

		if (m_tMTAnimClip.fUpdateTime >= m_tMTAnimClip.dTimeLength)
		{
			m_tMTAnimClip.fUpdateTime = 0.f;
		}

		m_dCurTime = m_tMTAnimClip.dStartTime + m_tMTAnimClip.fUpdateTime;

		// ���� ������ �ε��� ���ϱ�
		double dFrameIdx = m_dCurTime * (double)m_pOwner->m_iFrameCount;
		m_pOwner->m_iFrameIdx = (int)(dFrameIdx);

		// ���������� iframecount /

		// ���� ������ �ε���
		if (m_pOwner->m_iFrameIdx >= m_tMTAnimClip.iFrameLength - 1)
			m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx;	// ���̸� ���� �ε����� ����
		else
			m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx + 1;

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		m_pOwner->m_fRatio = (float)(dFrameIdx - (double)m_pOwner->m_iFrameIdx);

		// ��ǻƮ ���̴� ���꿩��
		m_pOwner->m_bFinalMatUpdate = false;
	}
}

void CAnim3D::SetFrame(int _Frame)
{
	// ���� �ð� ���ϱ�
	double dCurTime = _Frame / (double)m_pOwner->m_iFrameCount;
	m_tMTAnimClip.fUpdateTime = dCurTime - m_tMTAnimClip.dStartTime;

	if (m_tMTAnimClip.fUpdateTime >= m_tMTAnimClip.dTimeLength)
	{
		m_tMTAnimClip.fUpdateTime = 0.f;
	}

	m_dCurTime = m_tMTAnimClip.dStartTime + m_tMTAnimClip.fUpdateTime;

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dCurTime * (double)m_pOwner->m_iFrameCount;
	m_pOwner->m_iFrameIdx = (int)(dFrameIdx);

	// ���������� iframecount /

	// ���� ������ �ε���
	if (m_pOwner->m_iFrameIdx >= m_tMTAnimClip.iFrameLength - 1)
		m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
		m_pOwner->m_iNextFrameIdx = m_pOwner->m_iFrameIdx + 1;

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_pOwner->m_fRatio = (float)(dFrameIdx - (double)m_pOwner->m_iFrameIdx);

	// ��ǻƮ ���̴� ���꿩��
	m_pOwner->m_bFinalMatUpdate = false;
}