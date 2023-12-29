#pragma once
#include "Engine\CScript.h"

class CPlanetScript :
    public CScript
{
private:
    virtual void begin() override;
    virtual void tick() override;

public:
    CGameObject* m_pParent;    // �θ��༺
    Vec3    m_vStarPos;             // ������
    bool    m_bStar;                // �׼�

    float   m_fOrbitSpd;            // �����ӵ�
    float   m_fRotSpd;              // �����ӵ�
    Vec3    m_vOrbitAxis;           // ���� ��
    Vec3    m_vRotAxis;             // ���� ��
    float   m_fRadius;         

    void SetParentPlanet(CGameObject* _pObj) { m_pParent = _pObj; }

    void SetPlanet(float _fRadius, float _OrbitSpd, float _RotSpd, Vec3 _OrbitAxis, Vec3 _RotAxis)
    {
        m_fRadius = _fRadius;
        m_fOrbitSpd = _OrbitSpd;
        m_fRotSpd = _RotSpd;
        m_vOrbitAxis = _OrbitAxis;
        m_vRotAxis = _RotAxis;
    }

    void SetStar(bool _true) { m_bStar = _true; }
    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }
    void SetVelocity(float _fVelocity) { m_fOrbitSpd = _fVelocity; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    
    CLONE(CPlanetScript);

    CPlanetScript();
    ~CPlanetScript();
};

