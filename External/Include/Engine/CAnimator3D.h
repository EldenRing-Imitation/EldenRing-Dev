#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CBone.h"

class CStructuredBuffer;

class CAnim3D;

class CAnimator3D :
    public CComponent
{
private:
    // const vector<tMTBone>*      m_pVecBones;    // 사용안함
    
    map<wstring, CAnim3D*>      m_mapAnim;
    CAnim3D*                    m_pCurAnim;

    // vector<Matrix>				m_vecFinalBoneMat;          // 텍스쳐에 전달할 최종 행렬정보
    int							m_iFrameCount;              // 30

    // int							m_iFrameIdx;                // 클립의 현재 프레임
    // int							m_iNextFrameIdx;            // 클립의 다음 프레임
    // float						m_fRatio;	                // 프레임 사이 비율

    CStructuredBuffer*          m_pBoneFinalMatBuffer;      // 특정 프레임의 최종 행렬
    bool						m_bFinalMatUpdate;          // 최종행렬 연산 수행여부

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    // [delete]
    // void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    // void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount();
    void ClearData();

    map<wstring, CAnim3D*>& GetAnims() { return m_mapAnim; }
    CAnim3D* AddAnim(Ptr<CBone> _pBone);
    CAnim3D* GetCurAnim() { return m_pCurAnim; }

    // int GetCurFrameIdx() { return m_iFrameIdx; }

public:
    void Play();
    void Stop();
    void Reset();
    void SetFrame(int _Frame);

    CAnim3D* SelectAnimation(const wstring& _AnimName);

    bool IsPlay();

private:
    void Check_Bone(Ptr<CBone> _pBone);

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();

    friend class CAnim3D;
};