#include "stdafx.h"
#include "..\public\Level_Control.h"

#include "Level_Loading.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"

CLevel_Control::CLevel_Control(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CLevel_Control::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Control::Show_Window(void)
{
	ImGui::Begin("Level_Control");
	void BeginFrame();

	ImGui::PushItemWidth(200);
	
	ENGINE_OPEN;

	const char* items[2];
	items[0] = "Logo";
	items[1] = "Game_Play";


	static int item_current = 0;
	ImGui::ListBox("Sence", &item_current, items, IM_ARRAYSIZE(items), 2);

	switch (ENGINE->Get_NowLevel())
	{
	case LEVEL_LOADING:
		ImGui::Text("LOADING");
		break;

	case LEVEL_LOGO:
		ImGui::Text("LOGO");
		break;

	case LEVEL_GAMEPLAY:
		ImGui::Text("GAMEPLAY");
		break;
	}

	ImGui::Text("Selected : %s", items[item_current]);

	if (ImGui::Button("Go"))
	{
		if (0 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_LOGO));
		if (1 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_GAMEPLAY));
	
	}

	ENGINE_CLOSE;

	ImGui::End();

	__super::Show_Window();
}

CLevel_Control * CLevel_Control::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Control* pInstance = new CLevel_Control(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CLevel_Control");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Control::Free()
{
	__super::Free();

	for_each(m_ListBox.begin(), m_ListBox.end(), Safe_Delete_Array<char*>);
	m_ListBox.clear();
}
