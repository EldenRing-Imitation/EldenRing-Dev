#pragma once
#include "CEntity.h"
#include "CBone.h"
#include "ptr.h"

class CStructuredBuffer;

class CAnimator3D;

class CAnim3D :
    public CEntity
{
private:
    CAnimator3D*    m_pOwner;
	
	Ptr<CBone>		m_pBone;	// �ִϸ��̼� ��

	vector<Matrix>	m_vecFinalBoneMat;		// �ؽ��Ŀ� ������ ���� �������
	int				m_iFrameCount;			// 30

	int				m_iFrameIdx;			// Ŭ���� ���� ������
	int				m_iNextFrameIdx;		// Ŭ���� ���� ������
	float			m_fRatio;				// ������ ���� ����

	// Anim
	double			m_dCurTime;		// ����ð�

	tMTAnimClip		m_tMTAnimClip;

	bool			m_bPlay;		// ��� ����

public:
	void finaltick();

	Ptr<CBone> GetBone() { return m_pBone; }

	tMTAnimClip& GetAnimClip() { return m_tMTAnimClip; }
	double GetAnimTime() { return m_dCurTime; }

	const int GetCurFrameIdx() { return m_iFrameIdx; }
	const int GetNextFrameIdx() { return m_iNextFrameIdx; }

	bool IsPlay() { return m_bPlay; }

	void Play() { m_bPlay = true; }
	void Stop() { m_bPlay = false; }
	void Reset() { m_tMTAnimClip.fUpdateTime = 0.f; }

	void SetFrame(int _Frame);

	void SetOwner(CAnimator3D* _pOwner) { m_pOwner = _pOwner; }
	static CAnim3D* CreateAnimation(Ptr<CBone> _pBone);

	CLONE(CAnim3D);

public:
	CAnim3D();
	CAnim3D(const CAnim3D& _origin);
	~CAnim3D();

	friend class CAnimator3D;
};

