#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device :	public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	//enum MOUSEKEYSTATE { DIM_LB, , DIM_MB, DIM_END };
	/*enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };*/

private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long*)&m_MouseState) + eMoveState);
	}

public:
	HRESULT			Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	void			Invalidate_Input_Device(void);

public:
	_bool			Key_Down(_ubyte KeyState);
	_bool			Key_Up(_ubyte KeyState);
	_bool			Key_Pressing(_ubyte KeyState);
	_bool			Mouse_Down(MOUSEKEYSTATE MouseState);
	_bool			Mouse_Up(MOUSEKEYSTATE MouseState);
	_bool			Mouse_Pressing(MOUSEKEYSTATE MouseState);
	void			Reset_All(void);

private:
	_byte			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;

	_bool			m_bKeyState[256];
	_bool			m_bMouseState[3];

	_ubyte			m_LastInput;
		
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

	LPDIRECTINPUT8				m_pInputSDK;

public:
	virtual void		Free(void);
};

END