#include "pch.h"
#include "CUIComponent.h"

#include "CTransform.h"
#include "CKeyMgr.h"

#include "CScript.h"

CUIComponent::CUIComponent(COMPONENT_TYPE _type)
    : CComponent(_type)
    , m_bCsrPressed(false)
    , m_bCsrOnUI(false)
    , bOpen(false)
    , m_pUIScript(nullptr)
{
}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::CsrOnCheck()
{
    // �簢 �浹 : OBB ����
    CTransform* transform = GetOwner()->Transform();

    Vector2 UIPos = Vec2(transform->GetRelativePos().x, transform->GetRelativePos().y);
    Vector2 UIScale = Vec2(transform->GetRelativeScale().x, transform->GetRelativeScale().y);

    Vector2 MouseScale = { 1.f,1.f };
    Vector2 MousePos = CKeyMgr::GetInst()->GetUnProjectPos();
    Vector2 CenterDir = UIPos - MousePos; // �� ��ü�� �߽ɳ����� �Ÿ�����
    Vector2 Axis; // ���� ������
    float CenterProjDist; // ������ �������� �� �߽��� ������ �Ÿ� ��Į��
    float r1, r2; // �� ����� �� ������ ��������

    //  1. Rect�� right�� ����
    Axis = transform->GetRelativeDir(DIR_TYPE::RIGHT);
    CenterProjDist = abs(CenterDir.Dot(Axis));

    r1 = UIScale.x / 2.f;
    r2 = MouseScale.x / 2.f;

    if (r1 + r2 < CenterProjDist)
    {
        m_bCsrOnUI = false;
        return;
    }

    // 2. Rect�� up�� ����
    Axis = transform->GetRelativeDir(DIR_TYPE::UP);
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

void CUIComponent::begin()
{
}

void CUIComponent::tick()
{
}

void CUIComponent::finaltick()
{
    CsrOnCheck();
}

void CUIComponent::CsrOn()
{
    if (m_pUIScript && m_CsrOnFunc) 
        (m_pUIScript->*m_CsrOnFunc)();
}

void CUIComponent::CsrTap()
{
    m_bCsrPressed = true;  
    if (m_pUIScript && m_CsrTapFunc) 
        (m_pUIScript->*m_CsrTapFunc)();
}

void CUIComponent::CsrRelease()
{
    m_bCsrPressed = false; 
    if (m_pUIScript && m_CsrReleasFunc) 
        (m_pUIScript->*m_CsrReleasFunc)();
}

void CUIComponent::CsrClick()
{
    if (m_pUIScript && m_CsrClickFunc) 
        (m_pUIScript->*m_CsrClickFunc)();
}
