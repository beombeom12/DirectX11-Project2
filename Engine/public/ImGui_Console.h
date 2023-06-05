#pragma once

#include "Base.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "ImGuizmo.h"
#include "ImSequencer.h"

BEGIN(Engine)

class ENGINE_DLL CImGui_Console : public CBase
{
protected:
	CImGui_Console(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Console() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:
	list<char*>		CharList;
	list<_tchar*>	TcharList;

public:
	virtual void Free(void);
};

END