#pragma once

#include "Client_Defines.h"
#include "ImGui_Console.h"

BEGIN(Client)

class CLevel_Control : public CImGui_Console
{
private:
	CLevel_Control(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Control() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

private:
	list<char*> m_ListBox;

public:
	static CLevel_Control* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END