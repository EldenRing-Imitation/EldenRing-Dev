#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;

class CAnim3D;

class CAnimator3D :
    public CComponent
{
private:
    const vector<tMTBone>*      m_pVecBones;

    map<wstring, CAnim3D*>      m_mapAnim;
    CAnim3D*                    m_pCurAnim;

    vector<Matrix>				m_vecFinalBoneMat;          // �ؽ��Ŀ� ������ ���� �������
    int							m_iFrameCount;              // 30

    int							m_iFrameIdx;                // Ŭ���� ���� ������
    int							m_iNextFrameIdx;            // Ŭ���� ���� ������
    float						m_fRatio;	                // ������ ���� ����

    CStructuredBuffer*          m_pBoneFinalMatBuffer;      // Ư�� �������� ���� ���
    bool						m_bFinalMatUpdate;          // ������� ���� ���࿩��

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();

private:
    void check_mesh(Ptr<CMesh> _pMesh);

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