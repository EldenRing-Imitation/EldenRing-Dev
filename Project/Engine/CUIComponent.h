#pragma once
#include "CComponent.h"

class CScript;

class CUIComponent :
    public CComponent
{
private:
    bool m_bCsrPressed;
    
    bool m_bCsrOn;
    bool m_bPrevCsrOn;
    
    bool bOpen;

private:
    void CsrOnCheck();

public:
    virtual void begin() {};
    virtual void tick() {};
    virtual void finaltick();

public:
    bool IsCsrOn() { return m_bCsrOn; }
    bool IsCsrPressed() { return m_bCsrPressed; }

public:
    virtual void CsrOn();
    virtual void CsrTap();
    virtual void CsrRelease();
    virtual void CsrClick();
    virtual void CsrBeginOn();
    virtual void CsrAway();

public:
    virtual void SaveToLevelFile(FILE* _File) {}
    virtual void LoadFromLevelFile(FILE* _FILE) {}

    CLONE(CUIComponent);
public:
    CUIComponent();
    CUIComponent(const CUIComponent& _Origin);
    virtual ~CUIComponent();
};

