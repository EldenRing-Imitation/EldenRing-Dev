#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;

    Ptr<CMaterial>          m_pSharedMtrl;  // 원본 메테리얼
    Ptr<CMaterial>          m_pDynamicMtrl; // SharedMaterial 복사본
    Ptr<CMaterial>          m_pCurrentMtrl; // 현재 사용 중인 재질

    bool                    m_bFrustumCheck;

    float                   m_fBoundingBoxScale;
    float                   m_fBoundingBoxOffsetScale;

public:
    virtual void render() = 0;

public:
    void SetFrustumCheck(bool _Check) { m_bFrustumCheck = _Check; }

    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);
    void SetBoundingBoxScale(float _Radius) { m_fBoundingBoxOffsetScale = _Radius; }
    void SetBoundingBoxOffsetScale(float _Offset) { m_fBoundingBoxOffsetScale = _Offset; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();

    float GetBoundingBoxScale() { return m_fBoundingBoxScale + m_fBoundingBoxOffsetScale; }

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    bool IsFrustumCheck() { return m_bFrustumCheck; }


public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};

