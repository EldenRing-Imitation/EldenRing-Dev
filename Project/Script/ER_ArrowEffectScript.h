#pragma once
#include "ER_EffectScript.h"
class ER_ArrowEffectScript :
    public ER_EffectScript
{
private:
    CGameObject* m_pParticle;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SpawnEffect(Vec3 _pos = Vec3(0.f, 0.f, 0.f), Vec3 _dir = Vec3(0.f, 0.f, 0.f), float _scale = 0.f) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(ER_ArrowEffectScript);

    ER_ArrowEffectScript();
    ~ER_ArrowEffectScript();
};

