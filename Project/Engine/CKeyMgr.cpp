#include "pch.h"
#include "CKeyMgr.h"

#include "CRenderMgr.h"
#include "CCamera.h"

#include "CEngine.h"

int g_arrVK[(UINT)KEY::END]
=
{
	 VK_UP,
	 VK_DOWN,
	 VK_LEFT,
	 VK_RIGHT,

	 VK_SPACE,
	 VK_RETURN,
	 VK_ESCAPE,
	 VK_LMENU,
	 VK_LCONTROL,
	 VK_LSHIFT,
	 VK_TAB,

	 VK_LBUTTON,
	 VK_RBUTTON,

	 'Q',
	 'W',
	 'E',
	 'R',

	 'T',
	 'Y',
	 'U',
	 'I',
	 'O',
	 'P',

	 'A',
	 'S',
	 'D',
	 'F',

	 'Z',
	 'X',
	 'C',
	 'V',



	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,

	 VK_OEM_3,

	 0,
	 0,
};





CKeyMgr::CKeyMgr()
	: bCsrHide(false)
{

}

CKeyMgr::~CKeyMgr()
{
	Release();
}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i  , KEY_STATE::NONE });
	}
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		if(bCsrHide)
			ShowCursor(false);

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				// 이전에는 눌리지 않았다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::TAP;
					m_vecKey[i].bPrev = true;
				}
				else
				{
					// 지금도 눌려있고, 이전 프레임에서도 눌려있었다.
					m_vecKey[i].state = KEY_STATE::PRESSED;
				}
			}
			else
			{
				// 눌려있지 않다.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::NONE;
				}
				else
				{
					m_vecKey[i].state = KEY_STATE::RELEASE;
					m_vecKey[i].bPrev = false;
				}
			}
		}

		// Mouse 위치 갱신
		m_vPrevMousePos = m_vMousePos;

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
		m_vMousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);

		CalcUnProjectPos();

		m_vMouseDir = m_vMousePos - m_vPrevMousePos;
		m_vMouseDir.y *= -1;

		// 마우스 휠
		HRESULT hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
		if (FAILED(hr))
		{
			// 장치 상태 손실 시 장치 재활성화
			if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
				m_mouse->Acquire();
		}

		mouseWheelUp = false;
		mouseWheelDown = false;

		// 마우스 휠 값 확인
		if (m_mouseState.lZ > 0)
			mouseWheelUp = true; // 휠을 위로 스크롤할 때의 동작
		else if (m_mouseState.lZ < 0)
			mouseWheelDown = true; // 휠을 아래로 스크롤할 때의 동작

	}

	// Window 가 focus 상태가 아니다
	else
	{
		ShowCursor(true);
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].state || KEY_STATE::PRESSED == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::RELEASE;
			}

			else if (KEY_STATE::RELEASE == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::NONE;
			}
		}
	}

	
}

void CKeyMgr::DinputInit(HINSTANCE _hinstance, HWND _hwnd)
{
	if (FAILED(DirectInput8Create(_hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL)))
	{
		assert(NULL);
		return;
	}

;
	if (FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
	{
		assert(NULL);
		return;
	}

	;
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		assert(NULL);
		return;
	}

	if (FAILED(m_mouse->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		assert(NULL);
		return;
	}
	
	if (FAILED(m_mouse->Acquire()))
	{
		HRESULT hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
		if (FAILED(hr))
		{
			// 장치 상태 손실 시 장치 재활성화
			if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
				m_mouse->Acquire();
		}
	}

}

void CKeyMgr::Release()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

void CKeyMgr::CalcUnProjectPos()
{
	Vector3 ReturnPos(m_vMousePos.x, m_vMousePos.y, 0.f);

	RECT rect = {};
	GetClientRect(CEngine::GetInst()->GetMainWnd(), &rect);
	float width = (float)(rect.right - rect.left);
	float height = (float)(rect.bottom - rect.top);

	Viewport viewport;
	viewport.width = width;
	viewport.height = height;

	viewport.x = 0;
	viewport.y = 0;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	CCamera* pUICamera = CRenderMgr::GetInst()->GetUICam();

	if (!pUICamera)
		return;

	DirectX::XMFLOAT4X4 identityMatrix;
	DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());

	ReturnPos = viewport.Unproject(ReturnPos, pUICamera->GetProjMat(), pUICamera->GetViewMat(), identityMatrix);
	m_vUnProjectMousePos.x = ReturnPos.x;
	m_vUnProjectMousePos.y = ReturnPos.y;
}
