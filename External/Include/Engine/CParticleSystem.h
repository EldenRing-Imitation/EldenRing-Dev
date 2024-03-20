#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_RWBuffer;
    CStructuredBuffer*          m_ModuleDataBuffer;
    
    tParticleModule             m_ModuleData;
    Ptr<CParticleUpdateShader>  m_UpdateCS;

    bool                        m_bPosCustom;

    Vec3                        m_PointPos;
   
    float                       m_AccTime;

public:
    const int* GetModuleCheckList() { return m_ModuleData.ModuleCheck; }
    void ActiveModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = true; }
    void DeactivateModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = false; }

    tParticleModule& GetParticleInfo() { return m_ModuleData; }
    void SetParticleInfo(tParticleModule& _ParticleModule) { m_ModuleData = _ParticleModule; }

    // ===============
    // ��ƼŬ ���� ����
    // ===============
    
    void SetSpawnInitialColor(Vec3 _vInitialColor) { m_ModuleData.vSpawnColor = _vInitialColor; }   // ���� �� �ʱ� ���� ����
    void SetSpawnRate(int _SpawnRate) { m_ModuleData.SpawnRate = _SpawnRate; }                      // �ʴ� ���� ���� ����
    

    void SetMaxParticleCound(int _SpawnCnt) { m_ModuleData.iMaxParticleCount = _SpawnCnt; }         // �ִ� ��ƼŬ �ִ� ���� ����

    // ��ƼŬ ������ ���
    const tParticleModule& GetParticleData() { return m_ModuleData; }

public:
    void SetPosCustom(Vec3 _relativePos) { m_PointPos = _relativePos; }

    void GetRayPos();
    Vec3 GetWorldPos(Vec3 _relativePos, Vec3 _relrativeRot);

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};

