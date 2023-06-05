#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CImGui_Manager_Client : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager_Client)

private:
	explicit CImGui_Manager_Client();
	virtual ~CImGui_Manager_Client() = default;

public:
	HRESULT Ready_Imgui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_double TimeDelta);
	void	Late_Tick(_double TimeDelta);
	HRESULT Render();

public:
	void	CreateMainWindow();

public:
	void	IO_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Slider_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	//void	TreeNode_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Listbox_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Level_Status(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Math_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Phsx_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	UI_Setting(class CTransform* pTransform);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

public:
	static _bool	Show_IO_Test;
	static _bool	Show_Slider_Test;
	static _bool	Show_TreeNode_Test;
	static _bool	Show_ListBox_Test;
	static _bool	Show_Level_Status;
	static _bool	Show_Math_Test;
	static _bool	Show_Phsx_Text;
	static _bool	Show_UI_Setting;

private: //	Phsx
	_float4 CubePosition = _float4(0.f, 0.f, 0.f, 1.f);
	_float	PhsxGravity = 0.f;
	_float	PhsxWeight = 0.f;

private: //	Math
	_float4 XmFloat4_01 = _float4(0.f, 0.f, 0.f, 0.f);
	_float4 XmFloat4_02 = _float4(0.f, 0.f, 0.f, 0.f);
	_float4 XmFloat4_03 = _float4(0.f, 0.f, 0.f, 0.f);

	char	XmWhatResult[MAX_PATH]{};
	_float  XmResult = 0.f;

private: //	IOTEST
	char InputTest[MAX_PATH]{};
	char OutputTest[MAX_PATH]{};

private: //	SLIDERTEST
	_float Slider_01 = 0.f;
	_float Slider_02 = 0.f;
	_float Slider_03 = 0.f;

private: //	DragInt
	_int	DragInt = 0;
	_float  DragFloat = 0.f;

private:  //	ListBox
	list<char*> m_ListBox;

	char InputListBox[MAX_PATH]{};
	char SelectedListBox[MAX_PATH]{};
	char SearchListBox[MAX_PATH]{};

private: //	Heap Control
	list<char*>		CharList;
	list<_tchar*>	TcharList;

public:
	virtual void Free();
};

END