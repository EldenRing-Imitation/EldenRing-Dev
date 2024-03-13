#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    Vec3            m_vOffsetRot;

    bool            m_bAbsolute;
    COLLIDER2D_TYPE m_Shape;
    Matrix          m_matCollider2D;    // Collider 의 월드행렬

    int             m_iCollisionCount;  // 충돌 횟수


public:
    virtual void finaltick() override;

public:
    void SetOffsetPos(Vec3 _vOffsetPos) { m_vOffsetPos = Vec3(_vOffsetPos.x, _vOffsetPos.y, _vOffsetPos.z); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
    void SetOffsetRot(Vec3 _vOffsetRot) { m_vOffsetRot = _vOffsetRot; }
    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetCollider2DType(COLLIDER2D_TYPE _Type) { m_Shape = _Type; }

    const Matrix& GetColliderWorldMat() { return m_matCollider2D; }
    COLLIDER2D_TYPE GetCollider2DType() { return m_Shape; }


public:
    void BeginOverlap(CCollider2D* _Other);
    void OnOverlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCollider2D);
public:
    CCollider2D();
    ~CCollider2D();
};

