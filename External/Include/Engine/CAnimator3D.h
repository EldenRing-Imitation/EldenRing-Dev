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
    map<wstring, CAnim3D*>      m_mapAnim;
    CAnim3D*                    m_pCurAnim;

    int							m_iFrameCount;              // 30

    // default Animation
    int                         m_iFrameIdx;                // ���� Ŭ���� ������
    int                         m_iNextFrameIdx;            // Ŭ���� ���� ������
    float                       m_fRatio;                   // ������ ���� ����

    double                      m_dCurTime;                 // ���� �ð�

    bool                        m_bPlay;                    // ��� ����
    
    // Animation transitions Blending
    CAnim3D*                    m_pPreviousAnim;
    int                         m_iPreviousAnimFrmIdx;      // ���� Ŭ���� ��ȯ ��� ������
    bool                        m_bAnimTrans;               // �ִϸ��̼� ��ȯ ����

    double                      m_dTransitionUpdateTime;    // �ִϸ��̼� ���� �ð�
    float                       m_fTransitionTime;          // �ִϸ��̼� ��ȯ �ð�
    float                       m_fTransitionsRatio;        // �ִϸ��̼� ��ȯ ����
     
    CStructuredBuffer*          m_pBoneFinalMatBuffer;      // Ư�� �������� ���� ���
    bool						m_bFinalMatUpdate;          // ������� ���� ���࿩��

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount();
    void ClearData();

    map<wstring, CAnim3D*>& GetAnims() { return m_mapAnim; }
    CAnim3D* AddAnim(Ptr<CBone> _pBone);
    CAnim3D* GetPreviousAnim() { return m_pPreviousAnim; }
    CAnim3D* GetCurAnim() { return m_pCurAnim; }

    // int GetCurFrameIdx() { return m_iFrameIdx; }

public:
    void Play();
    void Stop();
    void Reset();
    void SetFrame(int _Frame);

    int GetCurFrame() { return m_iFrameIdx; }

    CAnim3D* SelectAnimation(const wstring& _AnimName);

    bool IsPlay();

private:
    void Check_Bone(Ptr<CBone> _pBone);

public:
    void SaveCurAnimDataToFile();

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