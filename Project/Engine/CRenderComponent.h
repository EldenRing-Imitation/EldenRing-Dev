#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"


struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // ���� ���׸���
    Ptr<CMaterial>  pDynamicMtrl;   // ���� ���׸����� ���纻    
    Ptr<CMaterial>  pCurMtrl;       // ���� ��� �� ���׸���
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;
    vector<tMtrlSet>        m_vecMtrls;     // ����    

    float                   m_fBounding;        // FrustumCheck �뵵 ������
    bool                    m_bDynamicShadow;

    Ptr<CMaterial>          m_pSharedMtrl;  // ���� ���׸���
    Ptr<CMaterial>          m_pDynamicMtrl; // SharedMaterial ���纻
    Ptr<CMaterial>          m_pCurrentMtrl;  // ���� ��� ���� ����

    bool                    m_bFrustumCheck; // ����ü �ø� üũ ����

    float                   m_fBoundingBoxScale;
    float                   m_fBoundingBoxOffsetScale;

public:
    virtual void render() = 0;
    virtual void render_shadowmap();

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);
    void SetFrustumCheck(bool _Check) { m_bFrustumCheck = _Check; }
    void SetBoundingBoxScale(float _Radius) { m_fBoundingBoxOffsetScale = _Radius; }
    void SetBoundingBoxOffsetScale(float _Offset) { m_fBoundingBoxOffsetScale = _Offset; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    bool IsUseFrustumCheck() { return m_bFrustumCheck; }    
    void SetBounding(float _fBounding) { m_fBounding = _fBounding; }
    float GetBounding() { return m_fBounding; }
    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }
    float GetBoundingBoxScale() { return m_fBoundingBoxScale + m_fBoundingBoxOffsetScale; }

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    bool IsFrustumCheck() { return m_bFrustumCheck; }

public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};

