#include "pch.h"
#include "CUIMgr.h"

#include "CKeyMgr.h"
#include "CLevelMgr.h"

#include "CTransform.h"
#include "CUIComponent.h"

#include "CLevel.h"
#include "CLayer.h"


CUIMgr::CUIMgr()
    : m_pFocusedUI(nullptr)
    , m_pPriorityUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::init()
{
}

void CUIMgr::tick()
{
	bool bCsrTap = KEY_TAP(KEY::LBTN);
	bool bCsrReleased = KEY_RELEASE(KEY::LBTN);

	// ���� ���� ����
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	
	// UI Layer ��������
	const vector<CGameObject*>& vecUI = pCurLevel->GetLayer(31)->GetParentObject();

	// �������� UI�� ���� �տ��ִ�(z���� ����) UI�� ��Ŀ�� UI�� �Ѵ�.
	m_pFocusedUI = nullptr;

	// Focused UI
	for (int i = (int)vecUI.size() - 1; 0 <= i; --i)
	{
		CGameObject* pOwner = vecUI[i];
		CUIComponent* pUI = pOwner->GetUIComponent();

		// UI Componenet�� ����
		if (!pUI)
			continue;
		
		if (pUI->IsCsrOn() && !(pOwner->IsDead()))
		{
			if (m_pFocusedUI == nullptr)
			{
				m_pFocusedUI = pOwner;
				continue;
			}

			if (m_pFocusedUI->GetUIComponent()->IsCsrOn())
			{
				float PrevFocusZ = m_pFocusedUI->Transform()->GetWorldPos().z;
				float NewFocusZ = pOwner->Transform()->GetWorldPos().z;

				if (PrevFocusZ > NewFocusZ)
					m_pFocusedUI = pOwner;
			}
			else
			{
				m_pFocusedUI = pOwner;
			}
		}
	}

	for (int i = (int)vecUI.size() - 1; 0 <= i; --i)
	{
		CGameObject* pOwner = vecUI[i];

		if (pOwner->GetName() == L"UI_ItemBox_BackGround" && KEY_TAP(KEY::RBTN))
			int a = 0;

		m_pPriorityUI = GetPriorityUI(pOwner); // ���� UI�� �켱���� UI�� �ִ��� Ȯ���Ѵ�.


		if (nullptr == m_pPriorityUI)	// �켱���� UI�� ���� ��� ���� UI �˻縦 ����
			continue;

		if (m_pFocusedUI != pOwner)		// �켱���� UI�� ��Ŀ�̵� UI�� �ƴѰ��
			continue;

		CUIComponent* pPriorityUI = m_pPriorityUI->GetUIComponent();

		// ���콺�� �̺�Ʈ ȣ��
		pPriorityUI->CsrOn();

		// �̹� update�� Lbtn�� ���ȴ�.
		if (bCsrTap)
		{
			// LbtnDoww �̺�Ʈ ȣ��
			pPriorityUI->CsrTap();
			break;

			// ���� ������ z���� ���� �����Ǿ� ���⼭ �������� �ʴ´�.
		}

		// �̹� update�� Lbtn�� ������.
		else if (bCsrReleased)
		{
			// ������ �������� �ִ�.
			if (pPriorityUI->IsCsrPressed())
			{
				// Ŭ�� �̺�Ʈ ȣ��
				pPriorityUI->CsrClick();
				pPriorityUI->CsrRelease();
			}
		}
	}
}

CGameObject* CUIMgr::GetPriorityUI(CGameObject* _pParentUI)
{
	bool bCsrReleased = KEY_RELEASE(KEY::LBTN);

	CUIComponent* pPriorityUI = nullptr;

	CGameObject* pParentUI = _pParentUI;
	
	if (!IsUIObject(_pParentUI))
		return nullptr;

	static queue<CGameObject*> _queue;
	_queue.push(pParentUI);				// ���ڷ� ���� �θ�UI�� ���� ���� �ִ´�.

	while (!_queue.empty()) // queue�� ��ü�� ���� ���������� �ݺ�
	{
		CGameObject* pUIObj = _queue.front(); // Queue�� ���� �տ� �ִ� UI�� �����´�.
		CUIComponent* pUI = pUIObj->GetUIComponent();

		_queue.pop(); // ������ UI�� Queue���� �����.

		const std::vector<CGameObject*>& ChildUI = pUIObj->GetChild(); // ������ UI�� �ڽ�UI���� �����´�.

		for (size_t i = 0; i < ChildUI.size(); ++i)
		{
			// UI Component�� �������̶�� Queue�� ���� �ִ´�.
			if(IsUIObject(ChildUI[i]))
				_queue.push(ChildUI[i]);
		}

		// UI�� ���� Ȯ��
		if (pUI->IsCsrOn())
		{
			// �켱���� UI�� ����������, ������ �켱������ ���� �ٸ� UI�� ���� ���.
			// ���� UI�� CsrPressed ���¸� �����Ѵ�.
			if (bCsrReleased && nullptr != pPriorityUI && pPriorityUI->IsCsrPressed())
			{
				pPriorityUI->CsrRelease();
			}

			// �켱���� UI�� ����
			pPriorityUI = pUI;
		}

		// �켱����UI�� ���� �ȵ� UI�� ���콺 ���������� �߻��ϸ� LbtnUp ���·� ���´�.
		else if (bCsrReleased && pUI->IsCsrPressed())
		{
			pUI->CsrRelease();
		}
	}

	if (nullptr == pPriorityUI)
		return nullptr;
	else
		return pPriorityUI->GetOwner();
}

bool CUIMgr::IsUIObject(CGameObject* _obj)
{
	return nullptr != _obj->GetUIComponent();
}
