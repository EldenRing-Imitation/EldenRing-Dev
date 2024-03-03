#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"

#include "CAnim3D.h"


CAnimator3D::CAnimator3D()
	: m_pCurAnim(nullptr)
	, m_iFrameCount(30)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_dCurTime(0.f)
	, m_bPlay(false)
	, m_pPreviousAnim(nullptr)
	, m_iPreviousAnimFrmIdx(0)
	, m_bAnimTrans(false)
	, m_dTransitionUpdateTime(0.f)
	, m_fTransitionTime(0.1f)
	, m_fTransitionsRatio(0.f)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_fTransitionTime = 5.f / m_iFrameCount;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pCurAnim(nullptr)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_dCurTime(0.f)
	, m_bPlay(false)
	, m_pPreviousAnim(nullptr)
	, m_iPreviousAnimFrmIdx(0)
	, m_bAnimTrans(false)
	, m_dTransitionUpdateTime(0.f)
	, m_fTransitionTime(_origin.m_fTransitionTime)
	, m_fTransitionsRatio(0.f)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	Safe_Del_Map<wstring, CAnim3D*>(m_mapAnim);
}


void CAnimator3D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		// �ִϸ��̼� ��ȯ
		if (m_bAnimTrans)
		{
			// - ���� : �ִϸ��̼� ��ȯ�� �߻������� ���� �ִϸ��̼� ������ ����.
			assert(m_pPreviousAnim);

			if (m_bPlay)
			{
				m_dTransitionUpdateTime += DT;

				if (m_fTransitionTime <= (float)m_dTransitionUpdateTime)	// ��ȯ�ð��� �Ѿ�� ��ȯ���� ����.
				{
					m_bAnimTrans = false;
					m_pPreviousAnim = nullptr;
				}
				else
				{
					// �ִϸ��̼� ��ȯ ������ �����ش�.
					m_fTransitionsRatio = (float)m_dTransitionUpdateTime / m_fTransitionTime;
					m_bFinalMatUpdate = false;
				}
			}
		}

		if (!m_bAnimTrans && m_bPlay)
		{
			// �ݺ�����, �ִϸ��̼� �ϷῩ�η� ���� �۾�
			// if (m_pCurAnim->IsFinish())
			// {
			// 	m_pCurAnim->Reset();
			// }

			CAnim3D* pCurAnim = m_pCurAnim;

			m_dCurTime = 0.f;

			// ���� ������� Clip �� �ð��� �����Ѵ�.
			pCurAnim->m_tMTAnimClip.fUpdateTime += DT;

			if (pCurAnim->m_tMTAnimClip.fUpdateTime >= pCurAnim->m_tMTAnimClip.dEndTime)
			{
				pCurAnim->m_tMTAnimClip.fUpdateTime = 0.f;
			}

			m_dCurTime = pCurAnim->m_tMTAnimClip.dStartTime + pCurAnim->m_tMTAnimClip.fUpdateTime;

			// ���� ������ �ε��� ���ϱ�
			double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
			m_iFrameIdx = (int)(dFrameIdx);

			// ���������� iframecount /

			// ���� ������ �ε���
			if (m_iFrameIdx >= pCurAnim->m_tMTAnimClip.iEndFrame - 1)
				m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
			else
				m_iNextFrameIdx = m_iFrameIdx + 1;

			// �����Ӱ��� �ð��� ���� ������ �����ش�.
			m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

			// ��ǻƮ ���̴� ���꿩��
			m_bFinalMatUpdate = false;
		}
	}
}

// void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
// {
// 	for (size_t i = 0; i < _vecAnimClip->size(); ++i)
// 	{
// 		CAnim3D* pAnim = new CAnim3D();
// 		pAnim->SetOwner(this);
// 
// 		tMTAnimClip tmp = _vecAnimClip->at(i);
// 		pAnim->Create((*_vecAnimClip)[i]);
// 
// 		m_mapAnim.insert(make_pair(tmp.strAnimName, pAnim));
// 		m_pCurAnim = pAnim;
// 	}
// }


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate && m_pCurAnim)
	{
		if (!m_bAnimTrans)
		{
			// Animation3D Update Compute Shader
			CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

			// Bone Data
			Ptr<CBone> pBone = m_pCurAnim->GetBone();
			Check_Bone(pBone);

			pUpdateShader->SetFrameDataBuffer(pBone->GetBoneFrameDataBuffer());
			pUpdateShader->SetOffsetMatBuffer(pBone->GetBoneOffsetBuffer());
			pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

			UINT iBoneCount = (UINT)pBone->GetBones()->size();
			pUpdateShader->SetBoneCount(iBoneCount);
			pUpdateShader->SetFrameIndex(m_iFrameIdx);
			pUpdateShader->SetFrameRatio(m_fRatio);
			pUpdateShader->SetIsAnimTrans(m_bAnimTrans);
			pUpdateShader->SetEndFrameIndex(m_pCurAnim->GetAnimClip().iEndFrame);

			// ������Ʈ ���̴� ����
			pUpdateShader->Execute();
		}
		else
		{
			 // Animation3D Update Compute Shader
			 CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();
			 
			 // ���� ��� Bone Data�� ���缭.
			 Ptr<CBone> pBone = m_pCurAnim->GetBone();
			 Check_Bone(pBone);

			 Ptr<CBone> pPreviousBone = m_pPreviousAnim->GetBone();
			 
			 pUpdateShader->SetFrameDataBuffer(pBone->GetBoneFrameDataBuffer());
			 pUpdateShader->SetPreviousFrameDataBuffer(pPreviousBone->GetBoneFrameDataBuffer());
			 pUpdateShader->SetOffsetMatBuffer(pBone->GetBoneOffsetBuffer());
			 pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);
			 
			 UINT iBoneCount = (UINT)pBone->GetBones()->size();

			 pUpdateShader->SetBoneCount(iBoneCount);
			 pUpdateShader->SetIsAnimTrans(m_bAnimTrans);
			 pUpdateShader->SetFrameIndex(m_iPreviousAnimFrmIdx);
			 pUpdateShader->SetFrameRatio(m_fTransitionsRatio);
			 pUpdateShader->SetEndFrameIndex(m_pCurAnim->GetAnimClip().iEndFrame);
			 
			 // ������Ʈ ���̴� ����
			 pUpdateShader->Execute();
		}

		m_bFinalMatUpdate = true;
	}

	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

UINT CAnimator3D::GetBoneCount()
{
	return m_pCurAnim->m_pBone->GetBoneCount(); 
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
		pMtrl->SetBoneCount(0);
	}
}

CAnim3D* CAnimator3D::AddAnim(Ptr<CBone> _pBone)
{
	map<wstring, CAnim3D*>::iterator iter = m_mapAnim.find(_pBone->GetKey());
	if(iter != m_mapAnim.end())
		return nullptr;
	CAnim3D* pAnim = new CAnim3D;
	pAnim = CAnim3D::CreateAnimation(_pBone);
	pAnim->SetOwner(this);
	m_mapAnim.insert(make_pair(_pBone->GetKey(), pAnim));
	pAnim->SetName(_pBone->GetKey());

	SelectAnimation(_pBone->GetKey());

	return pAnim;
}

void CAnimator3D::Play()
{
	if (m_pCurAnim)
		m_bPlay = true;
}

void CAnimator3D::Stop()
{
	if (m_pCurAnim)
		m_bPlay = false;
}

void CAnimator3D::Reset()
{
	if (m_pCurAnim)
		m_pCurAnim->Reset();
}

void CAnimator3D::SetFrame(int _Frame)
{
	if (m_pCurAnim)
	{
		// ���� �ð� ���ϱ�
		double dCurTime = _Frame / (double)m_iFrameCount;
		m_pCurAnim->m_tMTAnimClip.fUpdateTime = float(dCurTime - m_pCurAnim->m_tMTAnimClip.dStartTime);

		if (m_pCurAnim->m_tMTAnimClip.fUpdateTime >= m_pCurAnim->m_tMTAnimClip.dEndTime)
		{
			m_pCurAnim->m_tMTAnimClip.fUpdateTime = 0.f;
		}

		m_dCurTime = m_pCurAnim->m_tMTAnimClip.dStartTime + m_pCurAnim->m_tMTAnimClip.fUpdateTime;

		// ���� ������ �ε��� ���ϱ�
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		// ���������� iframecount /

		// ���� ������ �ε���
		if (m_iFrameIdx >= m_pCurAnim->m_tMTAnimClip.iEndFrame - 1)
			m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
		else
			m_iNextFrameIdx = m_iFrameIdx + 1;

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

		// ��ǻƮ ���̴� ���꿩��
		m_bFinalMatUpdate = false;
	}
}

CAnim3D* CAnimator3D::SelectAnimation(const wstring& _AnimName)
{
	// ������̴� �ִϸ��̼��� ������.
	if (!m_pCurAnim)
	{
		m_pCurAnim = m_mapAnim.find(_AnimName)->second;
		Play();
	}
	else
	{
		// ���� �ִϸ��̼� ����
		m_pPreviousAnim = m_pCurAnim;
		m_iPreviousAnimFrmIdx = m_iFrameIdx;
		
		// �ű� �ִϸ��̼� ���������� �ʱ�ȭ
		m_iFrameIdx = 0;
		m_iNextFrameIdx = 0;

		// �ű� �ִϸ��̼� ����
		m_pCurAnim = m_mapAnim.find(_AnimName)->second;
		m_pCurAnim->Reset();

		// �ִϸ��̼� ��ȯ ����
		m_bAnimTrans = 1;
		m_dTransitionUpdateTime = 0.f;
		m_fTransitionsRatio = 0.f;

		Play();
	}

	return m_pCurAnim;
}

bool CAnimator3D::IsPlay()
{
	if (m_pCurAnim)
		return m_bPlay;

	return false;
}

void CAnimator3D::Check_Bone(Ptr<CBone> _pBone)
{
	UINT iBoneCount = _pBone->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveCurAnimDataToFile()
{
	m_pCurAnim->GetBone()->SetAnimClip(m_pCurAnim->GetAnimClip());
	m_pCurAnim->GetBone()->Save(m_pCurAnim->GetBone()->GetKey());
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}

void CAnimator3D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CAnimator3D::LoadFromDB(int _gameObjectID)
{
}
