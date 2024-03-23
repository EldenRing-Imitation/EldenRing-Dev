#include "pch.h"
#include "CUIComponent.h"

#include "CTransform.h"
#include "CKeyMgr.h"

#include "CScript.h"

CUIComponent::CUIComponent()
    : CComponent(COMPONENT_TYPE::UICOMPONENT)
    , m_bCsrPressed(false)
    , m_bCsrOnUI(false)
    , bOpen(false)
{
}

CUIComponent::CUIComponent(const CUIComponent& _Origin)
    : CComponent(COMPONENT_TYPE::UICOMPONENT)
    , m_bCsrPressed(false)
    , m_bCsrOnUI(false)
    , bOpen(false)
{
}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::CsrOnCheck()
{
    if (GetOwner()->GetName() == L"ChildTestUI")
    {
        int stop = 1;
    }

    // �簢 �浹 : OBB ����
    Matrix WolrdMat = Transform()->GetWorldMat();
    Matrix WorldScaleMat = Transform()->GetWorldScaleMat();


    Vec2 UIPos(WolrdMat._41, WolrdMat._42);
    Vec2 UIScale(
        sqrt(WorldScaleMat._11 * WorldScaleMat._11 + WorldScaleMat._12 * WorldScaleMat._12 + WorldScaleMat._13 * WorldScaleMat._13),
        sqrt(WorldScaleMat._21 * WorldScaleMat._21 + WorldScaleMat._22 * WorldScaleMat._22 + WorldScaleMat._23 * WorldScaleMat._23));

    Vec2 MouseScale = { 1.f,1.f };
    Vec2 MousePos = CKeyMgr::GetInst()->GetUnProjectPos();
    Vec2 CenterDir = UIPos - MousePos; // �� ��ü�� �߽ɳ����� �Ÿ�����
    Vec2 Axis; // ���� ������
    float CenterProjDist; // ������ �������� �� �߽��� ������ �Ÿ� ��Į��
    float r1, r2; // �� ����� �� ������ ��������

    //  1. Rect�� right�� ����
    Axis = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    CenterProjDist = abs(CenterDir.Dot(Axis));

    r1 = UIScale.x / 2.f;
    r2 = MouseScale.x / 2.f;

    if (r1 + r2 < CenterProjDist)
    {
        m_bCsrOnUI = false;
        return;
    }

    // 2. Rect�� up�� ����
    Axis = Transform()->GetWorldDir(DIR_TYPE::UP);
    CenterProjDist = abs(CenterDir.Dot(Axis));

    r1 = UIScale.y / 2.f;
    r2 = MouseScale.y / 2.f;

    if (r1 + r2 < CenterProjDist)
    {
        m_bCsrOnUI = false;
        return;
    }

    m_bCsrOnUI = true;
}

void CUIComponent::finaltick()
{
    CsrOnCheck();
}

void CUIComponent::CsrOn()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrOn", "");
    // Script ȣ��
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrClick();
    }
}

void CUIComponent::CsrTap()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrTap", "");
    m_bCsrPressed = true;  
    // Script ȣ��
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrClick();
    }
}

void CUIComponent::CsrRelease()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrRelease", "");
    m_bCsrPressed = false; 
    // Script ȣ��
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrClick();
    }
}

void CUIComponent::CsrClick()
{
    DEBUG_LOG_COLLISION(ToString(GetOwner()->GetName()).c_str(), "CsrClick", "");
    // Script ȣ��
    const vector<CScript*>& vecScript = GetOwner()->GetScripts();
    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        vecScript[i]->CsrClick();
    }
}
