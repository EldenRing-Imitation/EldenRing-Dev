#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CRaycastShader.h"
#include "CTexture.h"

class CNaviMap :
    public CComponent
{
private:
    Ptr<CRaycastShader> m_pCSRaycast;//�����ɽ��� ���̴�
    CStructuredBuffer*  m_pCrossBuffer;//�����ɽ��� ��� ����

    Ptr<CTexture>       m_NaviMap; //�׺� �� �ؽ���

    bool                m_bTrigger;

    tNaviResult         m_sResultPos;

    float               m_fMinMaxArr[6];//���ʴ�� xMin, xMax, yMin, yMax, zMin, zMax���� ������.

    //�׺� �� �ؽ�Ʈ
    Ptr<CTexture>       m_pNaviTex;

public:
    CNaviMap();
    ~CNaviMap();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    void Raycasting();
    
public:
    tNaviResult GetRayResultPos() { return m_sResultPos; }

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);
    
    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType);
    virtual void LoadFromDB(int _gameObjectID);

    CLONE(CNaviMap);
};

