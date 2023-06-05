#include "..\Public\ImGui_Console.h"

CImGui_Console::CImGui_Console(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImGui_Console::Initialize(void)
{
	return S_OK;
}

void CImGui_Console::Show_Window()
{
	/*	현존 컨트롤러 모음
	
	@ Client
	1. Level_Control

	@ Engine
	1. Object_Control

	*/
}

void CImGui_Console::Free(void)
{
	for_each(CharList.begin(), CharList.end(), Safe_Delete_Array<char*>);
	CharList.clear();

	for_each(TcharList.begin(), TcharList.end(), Safe_Delete_Array<wchar_t*>);
	TcharList.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
