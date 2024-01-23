#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;

    bool            m_bDeferred;
    bool            m_bEmissive;

public:
    // Decal �������� ������ ���� �� ������ �ƴ���
    void SetDeferredDecal(bool _bDeferred);

    // ��Į�� �������� ����Ұ����� �ƴ���
    void ActivateEmissive(bool _bActivate) { m_bEmissive = _bActivate; }

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CDecal);
public:
    CDecal();
    ~CDecal();
};

