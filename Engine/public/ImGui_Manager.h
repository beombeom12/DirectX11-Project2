#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CImGui_Manager : public CBase
{

	DECLARE_SINGLETON(CImGui_Manager)

private:
	explicit CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Ready_Imgui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_double TimeDelta);
	void	Late_Tick(_double TimeDelta);
	HRESULT	Render(void);

public:
	void	CreateMainWindow();

public:
	void	IO_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Slider_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	TreeNode_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Listbox_Test(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);
	void	Level_Status(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pContext = nullptr);

private:
	ID3D11Device*			m_pDevice = nullptr;	//
	ID3D11DeviceContext*	m_pContext = nullptr;	//

public:
	HRESULT		Add_Console(const _tchar* pName, class CImGui_Console* pConsole);
	HRESULT		Delete_Console(const _tchar* pName);

private:
	map<const _tchar*, class CImGui_Console*>			m_mapConsole;
	typedef map<const _tchar*, class CImGui_Console*>	CONSOLE;
	class CImGui_Console*								Find_Console(const _tchar* pName);

public:
	static _bool	Show_IO_Test;
	static _bool	Show_Slider_Test;
	static _bool	Show_TreeNode_Test;
	static _bool	Show_ListBox_Test;
	static _bool	Show_Level_Status;

private:	//	IOTEST
	char InputTest[MAX_PATH]{};
	char OutputTest[MAX_PATH]{};

private:	//	SLIDERTEST
	_float	Slider_01 = 0.f;
	_float	Slider_02 = 0.f;
	_float	Slider_03 = 0.f;

private:	//	DragInt
	_int	DragInt = 0;
	_float	DragFloat = 0.f;

private:	//	ListBox
	list<char*>	m_ListBox;

	char InputListBox[MAX_PATH]{};
	char SelectedListBox[MAX_PATH]{};
	char SearchListBox[MAX_PATH]{};

private:	//	Heap Control
	list<char*>		CharList;
	list<_tchar*>	TcharList;

public:
	virtual void Free(void);
};

END