#include "Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
	: m_pInputSDK(nullptr), m_pKeyBoard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;


	// Ű���� ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	return S_OK;
}

void CInput_Device::Invalidate_Input_Device(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

_bool CInput_Device::Key_Down(_ubyte KeyState)
{
	if ((false == m_bKeyState[KeyState]) && (Get_DIKeyState(KeyState) & 0x80))
	{
		m_LastInput = KeyState;
		m_bKeyState[KeyState] = true;
		return true;
	}

	return false;
}

_bool CInput_Device::Key_Up(_ubyte KeyState)
{
	if ((true == m_bKeyState[KeyState]) && !(Get_DIKeyState(KeyState) & 0x80))
	{
		m_LastInput = KeyState;
		m_bKeyState[KeyState] = false;
		return true;
	}
	return false;
}

_bool CInput_Device::Key_Pressing(_ubyte KeyState)
{
	if (Get_DIKeyState(KeyState) & 0x80)
	{
		m_LastInput = KeyState;
		return true;
	}

	return false;
}

_bool CInput_Device::Mouse_Down(MOUSEKEYSTATE MouseState)
{
	if (!m_bMouseState[MouseState] && (m_MouseState.rgbButtons[MouseState] & 0x80))
	{
		m_bMouseState[MouseState] = true;
		return true;
	}

	return false;
}	

_bool CInput_Device::Mouse_Up(MOUSEKEYSTATE MouseState)
{
	if (m_bMouseState[MouseState] && !(m_MouseState.rgbButtons[MouseState] & 0x80))
	{
		m_bMouseState[MouseState] = false;
		return true;
	}

	return false;
}

_bool CInput_Device::Mouse_Pressing(MOUSEKEYSTATE MouseState)
{
	if (Get_DIMouseState(MouseState) & 0x80)
	{
		return true;
	}

	return false;
}

void CInput_Device::Reset_All(void)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_bMouseState[i] && !(m_MouseState.rgbButtons[i] & 0x80))
			m_bMouseState[i] = false;
	}

	for (int i = 0; i < 3; ++i)
	{
		if (!m_bMouseState[i] && (m_MouseState.rgbButtons[i] & 0x80))
			m_bMouseState[i] = true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (m_bKeyState[i] && !(Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = false;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (!m_bKeyState[i] && (Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = true;
	}
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
