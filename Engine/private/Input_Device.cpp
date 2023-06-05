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
	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;


	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
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
