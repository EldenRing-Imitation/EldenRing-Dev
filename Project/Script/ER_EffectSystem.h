#pragma once
#include <Engine\CSingleton.h>
#include "ER_struct.h"

class CGameObject;

class ER_EffectSystem :
    public CSingleton<ER_EffectSystem>
{
    SINGLE(ER_EffectSystem);
private:
    vector<CGameObject*>    m_vDeleteParticles;
    vector<float>           m_vParticleLifeTime;

    mutex                   m_aFuncMutex;

public:
    void init();
    void progress();

public:
    //�븻 ��Ʈ ����Ʈ ������
    void SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, CGameObject* _projectile = nullptr, Vec3 _pos = Vec3(0.f,0.f,0.f), Vec3 _dir = Vec3(0.f,0.f,0.f));
    //��ų ��Ʈ ����Ʈ ������
    void SpawnSkillHitEffect(CGameObject* _attacker, CGameObject* _hitter, const tSkill_Info* _skillInfo, CGameObject* _projectile = nullptr,  Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f));

//�븻 ��Ʈ ����Ʈ    
public:
    void SpawnRioHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);
    void SpawnAyaHitEffect(Vec3 _pos);

//��ų ��Ʈ ����Ʈ
public:
    void SpawnRioWHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir, CGameObject* _hitter);
    void SpawnRioRHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir);


public:
    void AddDeleteParticles(CGameObject* _obj, CGameObject* _parentObj = nullptr);
};

